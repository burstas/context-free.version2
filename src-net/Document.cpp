// Document.cpp
// this file is part of Context Free
// ---------------------
// Copyright (C) 2008 John Horigan - john@glyphic.com
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// 
// John Horigan can be contacted at john@glyphic.com or at
// John Horigan, 1209 Villa St., Mountain View, CA 94041-1123, USA
//
//

#include "StdAfx.h"
#include "Document.h"
#include "Form1.h"
#include "RenderSizeDialog.h"
#include "variation.h"
#include "WinCanvas.h"
#include "WinSystem.h"
#include "Rand48.h"
#include "tiledCanvas.h"
#include "resource.h"
#include "SaveImage.h"
#include "SVGCanvas.h"
#include "SaveMovie.h"
#include "qtCanvas.h"
#include "CFString.h"
#include "UploadDesign.h"
#include "upload.h"

using namespace ContextFreeNet;
using namespace System;
using namespace System::Data;
using namespace System::IO;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace Microsoft::Win32;
using namespace System::Runtime::InteropServices;
using namespace System::Collections;
using namespace System::Windows::Forms;


[DllImport("user32", CharSet = CharSet::Auto, SetLastError = false)]
extern "C" int SendMessage(int hWnd, unsigned int Msg, int wParam, int* lParam);

void Document::InitializeStuff()
{
    // catch form closing to handle worker thread and saving cfdg file if it 
    // is modified
    FormClosing += gcnew FormClosingEventHandler(this, &Document::FormIsClosing);

    // Some initialization needs to be deferred until all the handles are made
    moreInitHandler = gcnew EventHandler(this, &Document::moreInitialization);
    this->Load += moreInitHandler;

    // keep track of when the cfdg file is modified
    cfdgText->ModifiedChanged += gcnew EventHandler(this, &Document::modifiedCFDG);

    // get resize events so that the status and message text fields can stay the
    // right size
    toolStrip1->SizeChanged += gcnew EventHandler(this, &Document::stripSizeChanged);

    // get renderbox resize events so that its backing store can be remad
    mResizeDelay = 0;
    resizeTimer = gcnew System::Windows::Forms::Timer();
    resizeTimer->Interval = 100;
    resizeTimer->Tick += gcnew EventHandler(this, &Document::resizeTick);
    renderBox->Layout += gcnew LayoutEventHandler(this, &Document::renderSizeChanging);

    // enable/disable the cut/copy/past/etc menu commands
    menuEdit->DropDownOpened += gcnew EventHandler(this, &Document::menuEdit_Popup);

    // initialize the variation to a random value
    currentVariation = Variation::random(3);
    maxVariation = Variation::recommendedMax(6);
    minVariation = Variation::recommendedMin();

    // Setup splitters

    documentSplitter->SplitterDistance = (int)(documentSplitter->Size.Width * 
                                         Form1::prefs->DocumentSplitter);
    documentSplitter->SplitterMoved += gcnew SplitterEventHandler(this, &Document::splitterMoved);

    // setup the render parameters and the render thread
    renderParams = new RenderParameters();
    lastRenderWasSized = false;
    renderThread = gcnew BackgroundWorker();
    renderThread->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler(this, &Document::RenderCompleted);
    renderThread->DoWork += gcnew DoWorkEventHandler(this, &Document::RunRenderThread);

    // setup the timer for status update events
    statusTimer = gcnew System::Windows::Forms::Timer();
    statusTimer->Interval = 100;
    statusTimer->Tick += gcnew EventHandler(this, &Document::statusTick);

    mTileTransform = new agg::trans_affine();
}

void Document::DestroyStuff()
{
    delete mCanvas;
    delete mRenderer;
    delete mSystem;
    delete mEngine;
    delete mTiledCanvas;
    delete mSVGCanvas;
    delete mAnimationCanvas;
    delete renderParams;
    delete mTileTransform;
}

System::Void Document::moreInitialization(System::Object^ sender, System::EventArgs^ e)
{
    menuEFind->Click += gcnew EventHandler((Form1^)MdiParent, &Form1::FindReplace_Click);

    // shrink the tabs to a reasonable size
    int tabs[1] = {12};
    IntPtr hr = cfdgText->Handle; 
    int ret = ::SendMessage(hr.ToInt32(), WinSystem::EM_SETTABSTOPS, 0, nullptr);
    ret = ::SendMessage(hr.ToInt32(), WinSystem::EM_SETTABSTOPS, 1, tabs);

    cfdgText->Font = ((Form1^)MdiParent)->TextFont;
    ((Form1^)MdiParent)->TextFontChanged += gcnew EventHandler(this, &Document::textFontHandler);

    cfdgText->Invalidate();

    // enable/disable File->Revert menu command
    ((Form1^)MdiParent)->menuFile->DropDownOpened += 
        gcnew EventHandler(this, &Document::menuFile_Popup);

    // load the bitmaps used in the render button
    ImageList^ iList = toolStrip1->ImageList;
    if (iList == nullptr)
        iList = gcnew ImageList();
    imageListBase = iList->Images->Count;
    iList->Images->AddRange(Form1::busyList);
    toolStrip1->ImageList = iList;

    // get key events in the variation edit field to make sure that only valid
    // variations are entered
    variationEdit = gcnew TextBox();
    variationEdit->KeyDown += gcnew KeyEventHandler(this, &Document::variationKeyDown);
    variationEdit->KeyPress += gcnew KeyPressEventHandler(this, &Document::variationKeyPress);
    processVariationChange = gcnew EventHandler(this, &Document::variationChanged);

    // Replace the placeholder ToolStripTextBox with an encapsulated TextBox.
    // This is done to fix a .Net bug with the ToolStripTextBox::Textchanged
    // event not firing reliably.
    toolStrip1->Items->Insert(toolStrip1->Items->IndexOf(toolStripVariation), 
                              gcnew ToolStripControlHost(variationEdit));
    toolStrip1->Items->Remove(toolStripVariation); 
    variationEdit->MaxLength = 6;
    variationEdit->Name = L"variationEdit";
    variationEdit->MinimumSize = System::Drawing::Size(75, 25);
    variationEdit->MaximumSize = System::Drawing::Size(75, 25);
    variationEdit->TextChanged += processVariationChange;
    --currentVariation;
    NextVar_Click(nullptr, nullptr);

    // For some reason this is too soon to initialize the cfdg edit box or 
    // trigger a render if the Render-On-Open setting is true. So instead
    // we will ask the application idle handler to initialize the cfdg edit
    // box and render its contents in a very lazy fashion.
    if (reloadWhenReady) {
        idleAction = Form1::prefs->RenderOnOpen ? 
            Form1::IdleAction::LoadAndRender : Form1::IdleAction::Load;
    } else {
        idleAction = Form1::IdleAction::Clear;
    }
}

System::Void Document::menuFSave_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (isNamed) {
        try {
            StreamWriter sw(Name);
            sw.Write(cfdgText->Text);
            cfdgText->Modified = false;
        } catch (Exception^) {
            setMessageText("The file could not be written." );
        }
    } else {
        menuFSaveAs_Click(sender, e);
    }
}

System::Void Document::menuFSaveAs_Click(System::Object^ sender, System::EventArgs^ e)
{
    ((Form1^)MdiParent)->saveFileDialog1->Filter = "CFDG Files|*.cfdg";
    ((Form1^)MdiParent)->saveFileDialog1->Title = "Select a CFDG File";
    ((Form1^)MdiParent)->saveFileDialog1->FileName = Text;

    if (((Form1^)MdiParent)->saveFileDialog1->ShowDialog() == Windows::Forms::DialogResult::OK) {
        String^ oldName = Name;
        Name = ((Form1^)MdiParent)->saveFileDialog1->FileName;
        TabText = Path::GetFileName(((Form1^)MdiParent)->saveFileDialog1->FileName);
        Text = TabText;
        isNamed = true;
        ((Form1^)MdiParent)->mruManager->Add(Name);
        Form1::changeDocumentFile(oldName, Name, this);
        menuFSave_Click(sender, e);
    }
}

System::Void Document::FormIsClosing(Object^ sender, FormClosingEventArgs^ e)
{
    canceledByUser = false;

    // we can't close the window while the worker thread is running, so stop it
    if (renderThread->IsBusy) {
        // remember to close the window when the thread ends
        postAction = PostRenderAction::Close;
        if (mRenderer) mRenderer->requestStop = true;
        e->Cancel = true;
        return;
    }

    // If the thread is stopped and the cfdg file is up-to-date then close
    if (!cfdgText->Modified) return;

    // See if the user wants to close (lose changes), save changes and close,
    // or cancel the close.
    String^ msg = "Save changes in " + Text + "?";
    Windows::Forms::DialogResult r = Windows::Forms::MessageBox::Show(this, 
        msg, "Context Free", Windows::Forms::MessageBoxButtons::YesNoCancel);

    if (r == Windows::Forms::DialogResult::Cancel) {
        e->Cancel = true;
        // if the close was from an application exit and the user cancels it
        // then set this flag to cancel the exit
        canceledByUser = true;
        return;
    }

    if (r == Windows::Forms::DialogResult::Yes) {
        // save the file and then close
        menuFSave_Click(this, e);
    }
}

System::Void Document::menuFClose_Click(System::Object^ sender, System::EventArgs^ e)
{
    this->Close();
}

System::Void Document::menuFRevert_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (!isNamed || !cfdgText->Modified) return;

    Windows::Forms::DialogResult r = Windows::Forms::MessageBox::Show(this, 
        "Do you wish to revert and lose your changes?", "Context Free", 
        Windows::Forms::MessageBoxButtons::YesNo);

    if (r == Windows::Forms::DialogResult::Yes) {
        reload(false);
    }
}

// Load or reload the cfdg edit box from the documents named source, which
// is either a file path or the name of an example.
void Document::reload(bool justClear)
{
    if (justClear) {
        cfdgText->Text = String::Empty;
        cfdgText->Modified = false;
        return;
    }

    int pos = Form1::exampleSet->IndexOfKey(this->Name);
    if (pos >= 0) {
        cfdgText->Text = (String^)Form1::exampleSet->GetByIndex(pos);
        cfdgText->Modified = false;
        return;
    }

    try {
        StreamReader^ sr = gcnew StreamReader(Name);
        try {
            cfdgText->Text = sr->ReadToEnd();
            cfdgText->Modified = false;
        } finally {
            if ( sr )
                delete (IDisposable^)sr;
        }
    } catch (Exception^) {
        ((Form1^)MdiParent)->mruManager->Remove(Name);
        setMessageText("The file could not be read." );
    }
}

void Document::setMessageText(String^ txt)
{
    toolStripMessage->Text = txt;
    ((Form1^)MdiParent)->AddMessage(Text, txt);
}

void Document::setStatusText(String^ txt, bool sendToPane)
{
    toolStripStatus->Text = txt;
    if (sendToPane)
        ((Form1^)MdiParent)->AddMessage(Text, txt);
}

// Receive the Modify-fag-changed event from the cfdg edit box and merge the updated
// flag into the three local modify flags.
System::Void Document::modifiedCFDG(System::Object^ sender, System::EventArgs^ e)
{
    TabText = cfdgText->Modified ? (Text + "*") : Text;
}

System::Void Document::menuRRender_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (renderThread->IsBusy) {
        postAction = PostRenderAction::Render;
        return;
    }

    renderParams->initFromPrefs();
    renderParams->width = renderParams->height = 0;
    renderParams->suppressDisplay = false;
    renderParams->action = RenderParameters::RenderActions::Render;
    lastRenderWasSized = false;
    DoRender();
}

System::Void Document::menuRRenderSize_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (renderThread->IsBusy) {
        postAction = PostRenderAction::RenderSize;
        return;
    }

    RenderSizeDialog^ rs = gcnew RenderSizeDialog(renderParams);

    if (sender != menuRRenderSize || rs->ShowDialog() == Windows::Forms::DialogResult::OK) {
        if (sender != menuRRenderSize) 
            renderParams->saveToPrefs();
        renderParams->action = RenderParameters::RenderActions::Render;
        lastRenderWasSized = true;
        DoRender();
    }

    delete rs;
}

System::Void Document::menuRRenderAgain_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (renderThread->IsBusy) {
        postAction = PostRenderAction::RenderRepeat;
        renderParams->action = RenderParameters::RenderActions::Render;
        return;
    }

    if (!lastRenderWasSized) {
        renderParams->width = renderParams->height = 0;
    } else {
        NextVar_Click(nullptr, nullptr);
    }
    DoRender();
}

System::Void Document::menuRStop_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (mRenderer && renderThread->IsBusy) {
        mRenderer->requestStop = true;
        updateRenderButton();
    }
}

System::Void Document::menuRImage_Click(System::Object^ sender, System::EventArgs^ e)
{ 
    if (!mCanvas) {
        setMessageText("There is no image to save.");
        return;
    }

    if (renderThread->IsBusy) {
        postAction = PostRenderAction::SaveImage;
        return;
    }

    SaveImage^ saveImageDlg = gcnew SaveImage(mTiled, mRectangular,
        Path::GetFileNameWithoutExtension(Text) + ".png",
        ((Form1^)MdiParent)->saveDirectory);

    if (saveImageDlg->ShowTheDialog(this) == System::Windows::Forms::DialogResult::OK) {
        ((Form1^)MdiParent)->saveDirectory = 
            Path::GetDirectoryName(saveImageDlg->FileDlgFileName);
        bool rect = mTiled && mRectangular && saveImageDlg->checkRectangular->Checked;
        switch (saveImageDlg->FileDlgFilterIndex) {
            case 1: // PNG
                saveToPNGorJPEG(saveImageDlg->FileDlgFileName, nullptr, false, rect);
                break;
            case 2: // JPEG
                saveToPNGorJPEG(saveImageDlg->FileDlgFileName, nullptr, true, rect);
                break;
            case 3: // SVG
                saveToSVG(saveImageDlg->FileDlgFileName);
                break;
            default:
                break;
        }
    }
    delete saveImageDlg;
}

bool Document::saveToPNGorJPEG(String^ path, System::IO::Stream^ str, bool JPEG, bool rect)
{
    setMessageText("Saving image...");

    array<Imaging::ImageCodecInfo^>^ codecs = Imaging::ImageCodecInfo::GetImageEncoders();
    Imaging::ImageCodecInfo^ jpegCodec = nullptr;
    for each (Imaging::ImageCodecInfo^ codec in codecs) {
        if (codec->MimeType->Equals("image/jpeg"))
            jpegCodec = codec;
    }

    if (jpegCodec == nullptr) {
        setMessageText("Can't seem to find an image encoder.");
        return false;
    }

    Imaging::EncoderParameters^ iParams = gcnew Imaging::EncoderParameters(1);
	long long qual = (long long)(Form1::prefs->JPEGQuality);
    iParams->Param[0] = gcnew Imaging::EncoderParameter(Imaging::Encoder::Quality, qual);

    try {
        Bitmap^ bm = MakeBitmap(Form1::prefs->ImageCrop, mCanvas);

        if (rect) {
            Imaging::PixelFormat fmt = bm->PixelFormat;
            if (fmt == Imaging::PixelFormat::Format8bppIndexed)
                fmt = Imaging::PixelFormat::Format24bppRgb;

            Bitmap^ newBM = gcnew Bitmap(mRectangularSize.Width, 
                mRectangularSize.Height, fmt);
            Graphics^ g = Graphics::FromImage(newBM);
            g->Clear(Color::White);
            drawTiled(bm, newBM, g, 0, 0);

            delete g;
            delete bm;
            bm = newBM;
        }

        if (path != nullptr) {
            String^ fileName = path;
            if (Form1::prefs->ImageAppendVariation) {
                fileName = Path::GetDirectoryName(path) + "\\" +
                    Path::GetFileNameWithoutExtension(path) +
                    "-" + variationEdit->Text->ToLower() +  
                    Path::GetExtension(path);
            }

			if (JPEG) 
	            bm->Save(fileName, jpegCodec, iParams);
			else
				bm->Save(fileName, Imaging::ImageFormat::Png);
        } else if (str != nullptr) {
			if (JPEG)
	            bm->Save(str, jpegCodec, iParams);
			else
				bm->Save(str, Imaging::ImageFormat::Png);
        } else {
            setMessageText("Nowhere to save the image.");
        }
        delete bm;
        setMessageText("Image save complete.");
    } catch (Exception^) {
        setMessageText("Image save failed.");
		return false;
    }
	return true;
}

void Document::saveToSVG(String^ path)
{
    if (!mRenderer) return;

    setMessageText("Saving SVG file...");

    renderParams->action = RenderParameters::RenderActions::SaveSVG;

    IntPtr hPath = Marshal::StringToHGlobalAnsi(path);
    mSVGCanvas = new SVGCanvas((const char*)(hPath.ToPointer()), 
        renderParams->width, renderParams->height, Form1::prefs->ImageCrop);
    Marshal::FreeHGlobal(hPath);

    postAction = PostRenderAction::DoNothing;
    renderThread->RunWorkerAsync();
    statusTimer->Start();
    updateRenderButton();
}


System::Void Document::menuRMovie_Click(System::Object^ sender, System::EventArgs^ e)
{ 
    if (!mCanvas || !mRenderer) {
        setMessageText("There is no movie to save.");
        return;
    }

    if (renderThread->IsBusy) {
        postAction = PostRenderAction::SaveMovie;
        return;
    }

    // We have to create this early so that it can catch Movie Settings button events
    if (!qtCanvas::systemInit()) {
        setMessageText("QuickTime is not installed or can't be initialized.");
        return;
    }

    mAnimationCanvas = new qtCanvas(renderParams->width, renderParams->height);

    mAnimationCanvas->setPreview(mCanvas->mBM, mCanvas->mWidth, 
        mCanvas->mHeight, mCanvas->mStride, mCanvas->mPixelFormat);

    SaveMovie^ saveMovieDlg = gcnew SaveMovie(mAnimationCanvas,
        Path::GetFileNameWithoutExtension(Text) + ".mov",
        ((Form1^)MdiParent)->saveDirectory);
    saveMovieDlg->checkZoom->Enabled = !mTiled;

    if (saveMovieDlg->ShowTheDialog(this) == System::Windows::Forms::DialogResult::OK) {
        ((Form1^)MdiParent)->saveDirectory = 
            Path::GetDirectoryName(saveMovieDlg->FileDlgFileName);

        String^ fileName = saveMovieDlg->FileDlgFileName;
        if (Form1::prefs->ImageAppendVariation) {
            fileName = Path::GetDirectoryName(fileName) + "\\" +
                Path::GetFileNameWithoutExtension(fileName) +
                "-" + variationEdit->Text->ToLower() +  
                Path::GetExtension(fileName);
        }

        setMessageText("Saving QuickTime movie.");

        if (Form1::prefs->AnimateUnits == 0) {
            renderParams->animateFrameCount = 
                mAnimationCanvas->convertToFrames((float)(Form1::prefs->AnimateLength));
        } else {
            renderParams->animateFrameCount = Form1::prefs->AnimateLength;
        }
        renderParams->animateZoom = Form1::prefs->AnimateZoom;
        renderParams->action = RenderParameters::RenderActions::Animate;

        CFStringRef cfs;
        IntPtr hPath = Marshal::StringToHGlobalUni(fileName);
        wchar_t* path = (wchar_t*)(hPath.ToPointer());
        cfs = CFStringCreateWithCharacters(0, 
            (const UniChar*)path, wcslen(path));

        mAnimationCanvas->prepareMovie(cfs);
        Marshal::FreeHGlobal(hPath);

        postAction = PostRenderAction::DoNothing;
        renderThread->RunWorkerAsync();
        statusTimer->Start();
        updateRenderButton();
    }
}

System::Void Document::menuRUpload_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (!mCanvas) {
        setMessageText("Nothing to upload, try rendering first.");
        return;
    }

    Upload u;
    u.mCompression = mCanvas->colorCount256() ? Upload::CompressPNG8 : Upload::CompressJPEG;
    u.mTiled = mTiled;
    u.mRect = mTiled && mRectangular;
    u.mVariation = currentVariation;

    IntPtr hText = Marshal::StringToHGlobalAnsi(cfdgText->Text);
    u.mText = (const char*)(hText.ToPointer());
    u.mTextLen = strlen(u.mText);

    UploadDesign^ uploadWiz = gcnew UploadDesign(this, 
        Path::GetFileNameWithoutExtension(Text), &u);

    uploadWiz->ShowDialog(this);
    delete uploadWiz;
    Marshal::FreeHGlobal(hText);
}

System::Void Document::menuRLegacy_Click(System::Object^ sender, System::EventArgs^ e)
{ 
    useLegacyVariation = !useLegacyVariation;
    mUserChangedVariation = true;
    menuRLegacy->Checked = useLegacyVariation;
}

System::Void Document::menuEUndo_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (cfdgText->CanUndo)
        cfdgText->Undo();
}

System::Void Document::menuERedo_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (cfdgText->CanRedo)
        cfdgText->Redo();
}

System::Void Document::menuECut_Click(System::Object^  sender, System::EventArgs^  e)
{
    if (cfdgText->SelectionLength > 0)
        cfdgText->Cut();
}

System::Void Document::menuECopy_Click(System::Object^  sender, System::EventArgs^  e)
{
    if (cfdgText->SelectionLength > 0)
        cfdgText->Copy();
}

System::Void Document::menuEPaste_Click(System::Object^  sender, System::EventArgs^  e)
{
    if (Clipboard::GetDataObject()->GetDataPresent(DataFormats::Text))
        cfdgText->Paste();
}

System::Void Document::menuEDelete_Click(System::Object^  sender, System::EventArgs^  e)
{
    if (cfdgText->SelectionLength > 0)
        cfdgText->SelectedText = String::Empty;
}

System::Void Document::menuEFont_Click(System::Object^  sender, System::EventArgs^  e)
{
    FontDialog^ fd = gcnew FontDialog();
    fd->AllowVerticalFonts = false;
    fd->FontMustExist = true;
    fd->ShowApply = true;
    fd->ShowColor = false;
    fd->ShowEffects = false;
    fd->ShowHelp = false;
    fd->Font = ((Form1^)MdiParent)->TextFont;
    fd->Apply += gcnew EventHandler(this, &Document::applyFontHandler);
    System::Drawing::Font^ oldFont = cfdgText->Font;
    if (fd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
        ((Form1^)MdiParent)->TextFont = fd->Font;
    } else {
        if (!Object::ReferenceEquals(oldFont, cfdgText->Font)) 
            textFontHandler(this, e);
    }
    delete fd;
}
System::Void Document::applyFontHandler(System::Object^ sender, System::EventArgs^ e)
{
    bool mod = cfdgText->Modified;
    cfdgText->Font = ((FontDialog^)sender)->Font;
    if (mod != cfdgText->Modified)
        cfdgText->Modified = mod;
}

System::Void Document::textFontHandler(System::Object^ sender, System::EventArgs^ e)
{
    bool mod = cfdgText->Modified;
    cfdgText->Font = ((Form1^)MdiParent)->TextFont;
    if (mod != cfdgText->Modified)
        cfdgText->Modified = mod;
}

System::Void Document::menuFile_Popup(System::Object^  sender, System::EventArgs^  e)
{
    menuFRevert->Enabled = isNamed && cfdgText->Modified;
}

System::Void Document::menuEdit_Popup(System::Object^  sender, System::EventArgs^  e)
{
    menuECopy->Enabled = cfdgText->SelectionLength > 0;
    menuECut->Enabled = cfdgText->SelectionLength > 0;
    menuEPaste->Enabled = Clipboard::GetDataObject()->GetDataPresent(DataFormats::Text);
    menuEDelete->Enabled = cfdgText->SelectionLength > 0;
    menuEUndo->Enabled = cfdgText->CanUndo;
    menuERedo->Enabled = cfdgText->CanRedo;
}

System::Void Document::RenderButton_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (mRenderer && renderThread->IsBusy) {
        if (mRenderer->requestFinishUp) {
            mRenderer->requestStop = true;
        } else {
            mRenderer->requestFinishUp = true;
            updateRenderButton();
        }
    } else {
        menuRRender_Click(sender, e);
    }
}

System::Void Document::PrevVar_Click(System::Object^ sender, System::EventArgs^ e)
{
    mUserChangedVariation = true;
    --currentVariation;
    if (currentVariation > maxVariation || currentVariation < minVariation)
        currentVariation = maxVariation;
    char vChars[Variation::maxStringLength];
    Variation::toString(currentVariation, vChars, false);
    variationEdit->Text = gcnew String(vChars);
}

System::Void Document::NextVar_Click(System::Object^ sender, System::EventArgs^ e)
{
    mUserChangedVariation = true;
    ++currentVariation;
    if (currentVariation > maxVariation || currentVariation < minVariation)
        currentVariation = minVariation;
    char vChars[Variation::maxStringLength];
    Variation::toString(currentVariation, vChars, false);
    variationEdit->Text = gcnew String(vChars);
}

void Document::WndProc( Message% m )
{
    // Listen for operating system messages.
    switch (m.Msg) {
        case WinSystem::WM_USER_MESSAGE_UPDATE: 
            {
                char* msg = (char*)m.WParam.ToPointer();
                String^ msgText = gcnew String(msg);
                delete[] msg;
                setMessageText(msgText);
                break;
            }
        case WinSystem::WM_USER_STATUS_UPDATE:
            {
                WinSystem::Stats* stat = (WinSystem::Stats*)m.WParam.ToPointer();
                bool show = m.LParam.ToInt32() != 0;
                if (stat) {
                    String^ preText = show ? "rescaling - " : "";
                    if (stat->toDoCount > 0 || !mCanvas) {
                        setStatusText(preText + 
                            String::Format("{0:D} shapes and {1:D} expansions to do", 
                            stat->shapeCount, stat->toDoCount), false);
                    } else {
                        String^ endText = mTiledCanvas ? ", tiled" : "";
                        setStatusText(preText +
                            String::Format("{0:D} shapes, {1:D} x {2:D} pixels", 
                            stat->shapeCount, mCanvas->mWidth, mCanvas->mHeight)
                            + endText, !stat->inOutput);
                    }

                    if (stat->inOutput) {
                        if (mProgressDelay > 2) {
							int bar = (int)((100.0 * (double)stat->outputDone) / stat->outputCount);
							if (bar >= 0 && bar <= 100)
	                            toolStripProgressBar->Value = bar;
#ifdef DEBUG
							else {
								String^ oopsText = String::Format("Strange stats: shapes={0:D} todo={1:D} outputDone={2:D} outputCount={3:D}",
									stat->shapeCount, stat->toDoCount, stat->outputDone, stat->outputCount);
								System::Windows::Forms::MessageBox::Show(oopsText);
							}
#endif
                        } else {
                            ++mProgressDelay;
                            toolStripProgressBar->Value = 0;
                        }
                    } else {
                        mProgressDelay = 0;
                        toolStripProgressBar->Value = 0;
                    }
                } else {
                    updateRenderBox();
                }
                break;
            }
        case WinSystem::WM_USER_SYNTAX_ERROR:
            {
                try {
                    int line = (int)m.WParam.ToInt32() - 1;
                    int start = cfdgText->GetFirstCharIndexFromLine(line);
                    int end = cfdgText->GetFirstCharIndexFromLine(line + 1);
                    cfdgText->SelectionStart = start;
                    cfdgText->SelectionLength =  end - start;
                } catch (Exception^) {}
            }
    }
    WeifenLuo::WinFormsUI::Docking::DockContent::WndProc(m);
}

System::Void Document::stripSizeChanged(System::Object^ sender, System::EventArgs^ e)
{
    System::Drawing::Rectangle messageSize = toolStripMessage->Bounds;
    System::Drawing::Size stripSize = toolStrip1->Size;

    int newWidth = (stripSize.Width - messageSize.X - 4) / 2;
    toolStrip1->SuspendLayout();
    toolStripMessage->Width = newWidth;
    toolStripStatus->Width = newWidth;
    toolStrip1->ResumeLayout(true);
}

System::Void Document::renderSizeChanging(System::Object^ sender, System::Windows::Forms::LayoutEventArgs^ e)
{
    mResizeDelay = 0;
    resizeTimer->Enabled = true;
}

System::Void Document::resizeTick(Object^ sender, EventArgs^ e)
{
    ++mResizeDelay;
    if (mResizeDelay > 5) {
        resizeTimer->Stop();
        renderSizeChanged();
    }
}

void Document::renderSizeChanged()
{
    Bitmap^ oldImage = displayImage;
    System::Drawing::Size newSize = renderBox->Size;
    displayImage = gcnew Bitmap(newSize.Width, newSize.Height, 
        System::Drawing::Imaging::PixelFormat::Format32bppArgb);
    Graphics^ g = Graphics::FromImage(displayImage);
    g->Clear(Color::White);
    renderBox->Image = displayImage;
    updateRenderBox();
    delete oldImage;
}

System::Void Document::splitterMoved(Object^ sender, SplitterEventArgs^ e)
{
    Form1::prefs->DocumentSplitter = (double)(documentSplitter->SplitterDistance) /
                                     (double)(documentSplitter->Size.Width);
}

System::Void Document::variationKeyDown(Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
{
    nonAlphaInVariation = false;

    if (e->KeyCode >= Keys::A && e->KeyCode <= Keys::Z) return;
    if (e->KeyCode == Keys::Back) return;
    if (e->KeyCode == Keys::Return) return;

    nonAlphaInVariation = true;
}

System::Void Document::variationKeyPress(Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e)
{
    if (nonAlphaInVariation) {
        e->Handled = true;
        //Console::Beep();
        System::Media::SystemSounds::Beep->Play();
    } else if (e->KeyChar == '\r') {
        e->Handled = true;
        menuRRenderAgain_Click(sender, e);
    }
}

System::Void Document::variationChanged(System::Object^ sender, System::EventArgs^ e)
{
    if (variationEdit->Text == String::Empty) {
        variationEdit->Text = "A";
        System::Media::SystemSounds::Beep->Play();
        return;
    }

    mUserChangedVariation = true;
    IntPtr newText = Marshal::StringToHGlobalAnsi(variationEdit->Text);
    currentVariation = Variation::fromString((char*)(newText.ToPointer()));
    Marshal::FreeHGlobal(newText);
}

void Document::updateRenderButton()
{
    int lastIndex = mRenderButtonIndex;

    if (mRenderer && renderThread->IsBusy) {
        ++mRenderButtonIndex;
        if (mRenderButtonIndex > 8) mRenderButtonIndex = 1;
        if (mRenderer->requestFinishUp) {
            toolStripRenderButton->Text = "Stop Now";
        } else {
            toolStripRenderButton->Text = "Stop";
        }
    } else {
        toolStripRenderButton->Text = "Render";
        mRenderButtonIndex = 0;
    }

    if (lastIndex != mRenderButtonIndex) {
        toolStripRenderButton->ImageIndex = mRenderButtonIndex + imageListBase;
    }
}

void Document::DoRender()
{
    delete mEngine;
    delete mRenderer;
    mRenderer = 0;
    delete mTiledCanvas;
    mTiledCanvas = 0;

    Rand48::BigEndian = useLegacyVariation;

    if (!mSystem)
        mSystem = new WinSystem(this->Handle.ToPointer());

    bool modifiedSinceRender = SyncToSystem();

    if (!modifiedSinceRender && !mUserChangedVariation && !lastRenderWasSized) {
        NextVar_Click(nullptr, nullptr);
    }
    mUserChangedVariation = false;

    mEngine = CFDG::ParseFile(mSystem->mName.c_str(), mSystem, currentVariation);
    if (!mEngine) {
        System::Media::SystemSounds::Beep->Play();
        return;
    }

    int width = renderBox->Size.Width;
    int height = renderBox->Size.Height;

    mTiled = mEngine->isTiled(mTileTransform);
    if (mTiled) {
        width = (width * 9) / 10;
        height = (height * 9) / 10;
    }

    bool sizedRender = renderParams->width != 0 && renderParams->height != 0;
    if (sizedRender) {
        width = renderParams->width;
        height = renderParams->height;
    }

    mRenderer = mEngine->renderer(width, height, (float)renderParams->minimumSize, 
        currentVariation, renderParams->borderSize);
    renderParams->width = mRenderer->m_width;
    renderParams->height = mRenderer->m_height;

    delete mCanvas;
    mCanvas = 0;

    renderedWithLegacyVariation = useLegacyVariation;

    renderParams->periodicUpdate = renderParams->periodicUpdate && !sizedRender;
    if (renderParams->periodicUpdate)
        setupCanvas();

    if (!mCanvas || mCanvas->mWidth) {
        postAction = PostRenderAction::DoNothing;
        renderThread->RunWorkerAsync();
        statusTimer->Start();
        updateRenderButton();
    } else {
        delete mCanvas;
        mCanvas = 0;
    }
}

bool Document::SyncToSystem()
{
    IntPtr hName = Marshal::StringToHGlobalAnsi(this->Name);
    IntPtr hText = Marshal::StringToHGlobalAnsi(cfdgText->Text);
    bool ret = mSystem->updateInfo((const char*)(hName.ToPointer()), (const char*)(hText.ToPointer()));
    Marshal::FreeHGlobal(hName);
    Marshal::FreeHGlobal(hText);
    return ret;
}

void Document::RunRenderThread(Object^ sender, DoWorkEventArgs^ e)
{
    mProgressDelay = 0;
    switch (renderParams->action) {
        case RenderParameters::RenderActions::Animate: 
            {
                tiledCanvas* tile = 0;
                if (mTiled) {
                    tile = new tiledCanvas(mAnimationCanvas, *mTileTransform);
                    tile->scale(mScale);
                }
                mRenderer->animate(mTiled ? (Canvas*)tile : (Canvas*)mAnimationCanvas, 
                    renderParams->animateFrameCount, renderParams->animateZoom && !mTiled);

                delete tile;
                delete mAnimationCanvas;
                mAnimationCanvas = 0;
            }
            break;
        case RenderParameters::RenderActions::SaveSVG:
            mRenderer->draw(mSVGCanvas);
            delete mSVGCanvas;
            mSVGCanvas = 0;
            break;
        case RenderParameters::RenderActions::Render:
            {
                mScale = mRenderer->run(mTiled ? (Canvas*)mTiledCanvas : mCanvas, 
                    renderParams->periodicUpdate);
                if (!mCanvas) {
                    setupCanvas();
                    if (mTiledCanvas) mTiledCanvas->scale(mScale);
                    mRenderer->draw(mTiled ? (Canvas*)mTiledCanvas : mCanvas);
                }

                int x, y;
                mRectangular = mTiled && mTiledCanvas->isRectangular(&x, &y);
                if (mRectangular) {
                    if (x == 1 && y == 1) 
                        mRectangular = false;
                    mRectangularSize.Width = x * renderParams->width;
                    mRectangularSize.Height = y * renderParams->height;
                }
            }
            break;
    }
}

void Document::RenderCompleted(Object^ sender, RunWorkerCompletedEventArgs^ e)
{
    statusTimer->Stop();

    if (postAction == PostRenderAction::Exit) {
        MdiParent->Close();
        return;
    }

    if (postAction == PostRenderAction::Close) {
        this->Close();
        return;
    }

    updateRenderButton();

    switch (renderParams->action) {
        case RenderParameters::RenderActions::Render:
            if (renderParams->suppressDisplay)
                setMessageText("Done!");
            break;
        case RenderParameters::RenderActions::Animate:
            setMessageText("Movie save complete.");
            break;
        case RenderParameters::RenderActions::SaveSVG:
            setMessageText("SVG save complete.");
            break;
    }

    PostRenderAction nextAction = postAction;
    postAction = PostRenderAction::DoNothing;

    switch (nextAction) {
        case PostRenderAction::DoNothing:
            break;
        case PostRenderAction::Render:
            menuRRender->PerformClick();
            break;
        case PostRenderAction::RenderRepeat:
            this->menuRRenderAgain->PerformClick();
            break;
        case PostRenderAction::RenderSize:
            menuRRenderSize->PerformClick();
            break;
        case PostRenderAction::SaveImage:
            menuRImage->PerformClick();
            break;
        case PostRenderAction::SaveMovie:
            menuRMovie->PerformClick();
            break;
    }
}

System::Void Document::statusTick(Object^ sender, EventArgs^ e)
{
    if (renderThread->IsBusy && mRenderer) {
        updateRenderButton();
        mRenderer->requestUpdate = true;
    }
}

Bitmap^ Document::MakeBitmap(bool cropped, WinCanvas *canvas)
{
    Bitmap^ bm;
    char* data = canvas->mBM;
    int width = canvas->mWidth;
    int height = canvas->mHeight;
    if (cropped) {
        width = canvas->cropWidth();
        height = canvas->cropHeight();
        data += canvas->mStride * canvas->cropY() + 
                aggCanvas::BytesPerPixel[canvas->mPixelFormat] * canvas->cropX();
    }

    IntPtr pixelStore = IntPtr((void*)data);

    switch (canvas->mPixelFormat) {
        case aggCanvas::Gray8_Blend: {
                bm = gcnew Bitmap(width, height, canvas->mStride,Imaging::PixelFormat::Format8bppIndexed, pixelStore);
                Imaging::ColorPalette^ gray = bm->Palette;
                for (int i = 0; i < 256; ++i) {
                    gray->Entries[i] = Color::FromArgb(255, i, i, i);
                }
                bm->Palette = gray;
                break;
            }
        case aggCanvas::RGB8_Blend:
            bm = gcnew Bitmap(width, height, canvas->mStride, Imaging::PixelFormat::Format24bppRgb, pixelStore);
            break;
        case aggCanvas::RGBA8_Blend:
            bm = gcnew Bitmap(width, height, canvas->mStride, Imaging::PixelFormat::Format32bppPArgb, pixelStore);
            break;
        default:
            bm = nullptr;
            break;
    }

    return bm;
}

void Document::setupCanvas()
{
    if (displayImage == nullptr)
        renderSizeChanged();

    if (!mCanvas) {
        mCanvas = new WinCanvas(mSystem, WinCanvas::SuggestPixelFormat(mEngine), 
            renderParams->width, renderParams->height, mEngine->backgroundColor);
    }

    if (mTiled) {
        mTiledCanvas = new tiledCanvas(mCanvas, *mTileTransform);
    }
}

void Document::updateRenderBox()
{
    Graphics^ g = Graphics::FromImage(displayImage);

    if (!mCanvas) {
        g->Clear(Color::White);
        renderBox->Invalidate();
        return;
    }

    System::Drawing::Size destSize = displayImage->Size;
    System::Drawing::Size srcSize = System::Drawing::Size(mCanvas->mWidth, mCanvas->mHeight);

    double scale = 1.0;
    SolidBrush^ grayBrush = gcnew SolidBrush(Color::LightGray);
    Color back = Color::FromArgb(mCanvas->mBackground.a, 
        mCanvas->mBackground.r, mCanvas->mBackground.g, 
        mCanvas->mBackground.b);

    // check if the bitmap is too big and shrink it to fit
    if (srcSize.Width > destSize.Width || srcSize.Height > destSize.Height) {
        double widthScale = (double)destSize.Width / srcSize.Width;
        double heightScale = (double)destSize.Height / srcSize.Height;
        scale = (widthScale < heightScale) ? widthScale : heightScale;
    }

    // scale the bitmap if it is too big
    int scaledWidth = (int)(srcSize.Width * scale);
    int scaledHeight = (int)(srcSize.Height * scale);

    // center the scaled bitmap
    int originX = (destSize.Width - scaledWidth)/ 2;
    int originY = (destSize.Height - scaledHeight)/ 2;

    // Draw the bitmap scaled
    System::Drawing::Rectangle destRect(originX, originY, scaledWidth, scaledHeight);
    if (mCanvas->mBackground.a < 255) {
        g->Clear(Color::White);
        for (int y = 0; y <= (destSize.Height >> 3); y++)
            for (int x = 0; x <= (destSize.Width >> 3); x++)
                if ((x + y) & 1)
                    g->FillRectangle(grayBrush, x * 8, y * 8, 8, 8);

        if (!(mTiledCanvas && scale == 1.0) || renderParams->suppressDisplay) {
            SolidBrush^ b = gcnew SolidBrush(back);
            if (!(renderParams->suppressDisplay))
                g->SetClip(destRect, System::Drawing::Drawing2D::CombineMode::Exclude);
            g->FillRectangle(b, 0, 0, destSize.Width, destSize.Height);
            g->ResetClip();
        }
    } else {
        g->Clear(back);
    }

    if (renderParams->suppressDisplay) {
        renderBox->Invalidate();
        return;
    }

    Bitmap^ newBitmap = MakeBitmap(false, mCanvas);

    g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::HighQualityBicubic;

    if (mTiledCanvas && scale == 1.0) {
        drawTiled(newBitmap, displayImage, g, originX, originY);
    } else if (scale == 1.0) {
        g->DrawImageUnscaled(newBitmap, originX, originY);
    } else {
        g->DrawImage(newBitmap, destRect, 
            0, 0, srcSize.Width, srcSize.Height, 
            System::Drawing::GraphicsUnit::Pixel);
    }

    System::Drawing::Pen^ p2 = gcnew System::Drawing::Pen(Color::Black, 1.0);
    p2->DashStyle = System::Drawing::Drawing2D::DashStyle::Dot;
    g->DrawRectangle(p2, originX - 1, originY - 1, scaledWidth + 1, scaledHeight + 1);

    delete newBitmap;
    delete g;

    renderBox->Invalidate();
}

void Document::drawTiled(Bitmap^ src, Bitmap^ dest, Graphics^ g, int x, int y)
{
    tileList points = 
        mTiledCanvas->getTesselation(dest->Width, dest->Height,
        x, y, true);

    for each (agg::point_i pt in points) {
        g->DrawImageUnscaled(src, pt.x, pt.y);
    }
}
