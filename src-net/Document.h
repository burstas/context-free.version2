// Document.h
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

#pragma once

#include "Form1.h"
#include "RenderSizeDialog.h"

class WinCanvas;
class Canvas;
class CFDG;
class Renderer;
class tiledCanvas;
class WinSystem;
class SVGCanvas;
class qtCanvas;

namespace agg {
    struct trans_affine;
}

namespace ContextFreeNet {

	/// <summary>
	/// Summary for Document
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
    public ref class Document : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
        Document(void) :
              reloadWhenReady(false),
              idleAction(Form1::IdleAction::Nothing),
              useLegacyVariation(false),
              renderedWithLegacyVariation(false),
              mSystem(0),
              mRenderButtonIndex(0),
              mUserChangedVariation(true),
              mEngine(0),
              mRenderer(0),
              mCanvas(0),
              mTiledCanvas(0),
              mSVGCanvas(0),
              mAnimationCanvas(0),
              mPostAction(PostRenderAction::DoNothing)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            InitializeStuff();
		}
        bool isNamed;
        bool reloadWhenReady;
        Form1::IdleAction idleAction;
        int  currentVariation;
        int maxVariation, minVariation;
        bool useLegacyVariation;
        bool renderedWithLegacyVariation;
        RenderParameters* renderParams;
        bool lastRenderWasSized;

        enum class PostRenderAction { DoNothing, Render, RenderSize, RenderRepeat, 
            SaveImage, SaveMovie, Close, Exit };
        property PostRenderAction postAction {
            void set(PostRenderAction a) {
                if (a == PostRenderAction::DoNothing || a == PostRenderAction::Exit) {
                    mPostAction = a;
                } else if (mPostAction != PostRenderAction::Close && 
                           mPostAction != PostRenderAction::Exit) {
                    mPostAction = a;
                }
                if (!renderThread->IsBusy && mPostAction != PostRenderAction::DoNothing) {
                    RenderCompleted(nullptr, nullptr);
                }
            }
            PostRenderAction get() { return mPostAction; }
        }
        bool canceledByUser;

    private:
        PostRenderAction mPostAction;
        bool nonAlphaInVariation;
        WinSystem* mSystem;
        System::ComponentModel::BackgroundWorker^ renderThread;
        System::Windows::Forms::TextBox^ variationEdit;

    private: System::Windows::Forms::ToolStripLabel^  toolStripMessage;
    private: System::Windows::Forms::ToolStripLabel^  toolStripStatus;
    private: System::Windows::Forms::PictureBox^  renderBox;
    private: System::Windows::Forms::ToolStripMenuItem^  menuEUndo;
    private: System::Windows::Forms::ToolStripMenuItem^  menuERedo;


    private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator5;
    private: System::Windows::Forms::ToolStripMenuItem^  menuEDelete;
    private: System::Windows::Forms::ToolStripButton^  toolStripSaveButton;
    private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator6;
    private: System::Windows::Forms::ToolStripMenuItem^  menuEFont;
    private: System::Windows::Forms::ToolStripMenuItem^  menuEFind;








    public:
        System::String^ Name;
        void setMessageText(System::String^ txt);
        void setStatusText(System::String^ txt, bool addToMessages);
    public: void reload(bool justClear);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Document()
		{
			if (components)
			{
				delete components;
			}
            DestroyStuff();
		}
	private: System::Windows::Forms::ToolStrip^  toolStrip1;
    private: System::Windows::Forms::SplitContainer^  documentSplitter;

	public:  System::Windows::Forms::RichTextBox^  cfdgText;


	private: System::Windows::Forms::ToolStripButton^  toolStripRenderButton;

	private: System::Windows::Forms::ToolStripTextBox^  toolStripVariation;
	private: System::Windows::Forms::ToolStripButton^  toolStripPrevVar;
	private: System::Windows::Forms::ToolStripButton^  toolStripNextVar;

	private: System::Windows::Forms::ToolStripProgressBar^  toolStripProgressBar;


	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
    private: System::Windows::Forms::ToolStripMenuItem^  menuFile;

    private: System::Windows::Forms::ToolStripMenuItem^  menuEdit;


    private: System::Windows::Forms::ToolStripMenuItem^  menuECut;
    private: System::Windows::Forms::ToolStripMenuItem^  menuECopy;
    private: System::Windows::Forms::ToolStripMenuItem^  menuEPaste;



    private: System::Windows::Forms::ToolStripMenuItem^  menuFSave;
    private: System::Windows::Forms::ToolStripMenuItem^  menuFClose;


    private: System::Windows::Forms::ToolStripMenuItem^  menuFSaveAs;
    private: System::Windows::Forms::ToolStripMenuItem^  menuRender;


    public: System::Windows::Forms::ToolStripMenuItem^  menuRRender;
    private: System::Windows::Forms::ToolStripMenuItem^  menuRRenderSize;
    private: System::Windows::Forms::ToolStripMenuItem^  menuRRenderAgain;
    private: System::Windows::Forms::ToolStripMenuItem^  menuRStop;




    private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
    private: System::Windows::Forms::ToolStripMenuItem^  menuRImage;
    private: System::Windows::Forms::ToolStripMenuItem^  menuRMovie;
    private: System::Windows::Forms::ToolStripMenuItem^  menuRUpload;



    private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator4;

    private: System::Windows::Forms::ToolStripMenuItem^  menuRLegacy;
    private: System::Windows::Forms::ToolStripMenuItem^  menuFRevert;










	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Document::typeid));
            this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
            this->toolStripRenderButton = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolStripSaveButton = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolStripProgressBar = (gcnew System::Windows::Forms::ToolStripProgressBar());
            this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->toolStripVariation = (gcnew System::Windows::Forms::ToolStripTextBox());
            this->toolStripPrevVar = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolStripNextVar = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->toolStripMessage = (gcnew System::Windows::Forms::ToolStripLabel());
            this->toolStripStatus = (gcnew System::Windows::Forms::ToolStripLabel());
            this->documentSplitter = (gcnew System::Windows::Forms::SplitContainer());
            this->cfdgText = (gcnew System::Windows::Forms::RichTextBox());
            this->renderBox = (gcnew System::Windows::Forms::PictureBox());
            this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
            this->menuFile = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuFSave = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuFSaveAs = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuFClose = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuFRevert = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuEdit = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuEUndo = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuERedo = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator5 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->menuECut = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuECopy = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuEPaste = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuEDelete = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator6 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->menuEFont = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuEFind = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuRender = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuRRender = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuRRenderSize = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuRRenderAgain = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuRStop = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->menuRImage = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuRMovie = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuRUpload = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->menuRLegacy = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStrip1->SuspendLayout();
            this->documentSplitter->Panel1->SuspendLayout();
            this->documentSplitter->Panel2->SuspendLayout();
            this->documentSplitter->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->renderBox))->BeginInit();
            this->menuStrip1->SuspendLayout();
            this->SuspendLayout();
            // 
            // toolStrip1
            // 
            this->toolStrip1->GripStyle = System::Windows::Forms::ToolStripGripStyle::Hidden;
            this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(10) {this->toolStripRenderButton, 
                this->toolStripSaveButton, this->toolStripProgressBar, this->toolStripSeparator1, this->toolStripVariation, this->toolStripPrevVar, 
                this->toolStripNextVar, this->toolStripSeparator2, this->toolStripMessage, this->toolStripStatus});
            this->toolStrip1->Location = System::Drawing::Point(0, 0);
            this->toolStrip1->Name = L"toolStrip1";
            this->toolStrip1->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
            this->toolStrip1->Size = System::Drawing::Size(820, 25);
            this->toolStrip1->TabIndex = 0;
            this->toolStrip1->Text = L"CFDGstrip";
            // 
            // toolStripRenderButton
            // 
            this->toolStripRenderButton->AutoSize = false;
            this->toolStripRenderButton->AutoToolTip = false;
            this->toolStripRenderButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripRenderButton.Image")));
            this->toolStripRenderButton->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
            this->toolStripRenderButton->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->toolStripRenderButton->Name = L"toolStripRenderButton";
            this->toolStripRenderButton->Size = System::Drawing::Size(73, 22);
            this->toolStripRenderButton->Text = L"Render";
            this->toolStripRenderButton->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
            this->toolStripRenderButton->Click += gcnew System::EventHandler(this, &Document::RenderButton_Click);
            // 
            // toolStripSaveButton
            // 
            this->toolStripSaveButton->AutoToolTip = false;
            this->toolStripSaveButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
            this->toolStripSaveButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripSaveButton.Image")));
            this->toolStripSaveButton->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->toolStripSaveButton->Name = L"toolStripSaveButton";
            this->toolStripSaveButton->Size = System::Drawing::Size(71, 22);
            this->toolStripSaveButton->Text = L"Save Image";
            this->toolStripSaveButton->Click += gcnew System::EventHandler(this, &Document::menuRImage_Click);
            // 
            // toolStripProgressBar
            // 
            this->toolStripProgressBar->Name = L"toolStripProgressBar";
            this->toolStripProgressBar->Size = System::Drawing::Size(100, 22);
            // 
            // toolStripSeparator1
            // 
            this->toolStripSeparator1->Name = L"toolStripSeparator1";
            this->toolStripSeparator1->Size = System::Drawing::Size(6, 25);
            // 
            // toolStripVariation
            // 
            this->toolStripVariation->AutoSize = false;
            this->toolStripVariation->MaxLength = 6;
            this->toolStripVariation->Name = L"toolStripVariation";
            this->toolStripVariation->Size = System::Drawing::Size(75, 25);
            // 
            // toolStripPrevVar
            // 
            this->toolStripPrevVar->AutoToolTip = false;
            this->toolStripPrevVar->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->toolStripPrevVar->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripPrevVar.Image")));
            this->toolStripPrevVar->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->toolStripPrevVar->Name = L"toolStripPrevVar";
            this->toolStripPrevVar->Size = System::Drawing::Size(23, 22);
            this->toolStripPrevVar->Text = L"toolStripButton3";
            this->toolStripPrevVar->ToolTipText = L"Previous Variation";
            this->toolStripPrevVar->Click += gcnew System::EventHandler(this, &Document::PrevVar_Click);
            // 
            // toolStripNextVar
            // 
            this->toolStripNextVar->AutoToolTip = false;
            this->toolStripNextVar->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->toolStripNextVar->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripNextVar.Image")));
            this->toolStripNextVar->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->toolStripNextVar->Name = L"toolStripNextVar";
            this->toolStripNextVar->Size = System::Drawing::Size(23, 22);
            this->toolStripNextVar->Text = L"toolStripButton4";
            this->toolStripNextVar->ToolTipText = L"Next Variation";
            this->toolStripNextVar->Click += gcnew System::EventHandler(this, &Document::NextVar_Click);
            // 
            // toolStripSeparator2
            // 
            this->toolStripSeparator2->Name = L"toolStripSeparator2";
            this->toolStripSeparator2->Size = System::Drawing::Size(6, 25);
            // 
            // toolStripMessage
            // 
            this->toolStripMessage->AutoSize = false;
            this->toolStripMessage->AutoToolTip = true;
            this->toolStripMessage->Name = L"toolStripMessage";
            this->toolStripMessage->Size = System::Drawing::Size(200, 22);
            this->toolStripMessage->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
            // 
            // toolStripStatus
            // 
            this->toolStripStatus->AutoSize = false;
            this->toolStripStatus->AutoToolTip = true;
            this->toolStripStatus->Name = L"toolStripStatus";
            this->toolStripStatus->Size = System::Drawing::Size(200, 22);
            this->toolStripStatus->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
            // 
            // documentSplitter
            // 
            this->documentSplitter->Dock = System::Windows::Forms::DockStyle::Fill;
            this->documentSplitter->Location = System::Drawing::Point(0, 25);
            this->documentSplitter->Name = L"documentSplitter";
            // 
            // documentSplitter.Panel1
            // 
            this->documentSplitter->Panel1->Controls->Add(this->cfdgText);
            // 
            // documentSplitter.Panel2
            // 
            this->documentSplitter->Panel2->BackColor = System::Drawing::SystemColors::Window;
            this->documentSplitter->Panel2->Controls->Add(this->renderBox);
            this->documentSplitter->Size = System::Drawing::Size(820, 379);
            this->documentSplitter->SplitterDistance = 273;
            this->documentSplitter->TabIndex = 1;
            // 
            // cfdgText
            // 
            this->cfdgText->AcceptsTab = true;
            this->cfdgText->Dock = System::Windows::Forms::DockStyle::Fill;
            this->cfdgText->Location = System::Drawing::Point(0, 0);
            this->cfdgText->MaxLength = 1000000;
            this->cfdgText->Name = L"cfdgText";
            this->cfdgText->Size = System::Drawing::Size(273, 379);
            this->cfdgText->TabIndex = 0;
            this->cfdgText->Text = L"";
            this->cfdgText->WordWrap = false;
            // 
            // renderBox
            // 
            this->renderBox->Dock = System::Windows::Forms::DockStyle::Fill;
            this->renderBox->InitialImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"renderBox.InitialImage")));
            this->renderBox->Location = System::Drawing::Point(0, 0);
            this->renderBox->Name = L"renderBox";
            this->renderBox->Size = System::Drawing::Size(543, 379);
            this->renderBox->SizeMode = System::Windows::Forms::PictureBoxSizeMode::CenterImage;
            this->renderBox->TabIndex = 0;
            this->renderBox->TabStop = false;
            this->renderBox->Click += gcnew System::EventHandler(this, &Document::renderBox_Click);
            // 
            // menuStrip1
            // 
            this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->menuFile, this->menuEdit, 
                this->menuRender});
            this->menuStrip1->Location = System::Drawing::Point(0, 0);
            this->menuStrip1->Name = L"menuStrip1";
            this->menuStrip1->Size = System::Drawing::Size(820, 24);
            this->menuStrip1->TabIndex = 2;
            this->menuStrip1->Text = L"menuStrip1";
            this->menuStrip1->Visible = false;
            // 
            // menuFile
            // 
            this->menuFile->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->menuFSave, 
                this->menuFSaveAs, this->menuFClose, this->menuFRevert});
            this->menuFile->MergeAction = System::Windows::Forms::MergeAction::MatchOnly;
            this->menuFile->Name = L"menuFile";
            this->menuFile->Size = System::Drawing::Size(37, 20);
            this->menuFile->Text = L"&File";
            // 
            // menuFSave
            // 
            this->menuFSave->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"menuFSave.Image")));
            this->menuFSave->MergeAction = System::Windows::Forms::MergeAction::Insert;
            this->menuFSave->MergeIndex = 2;
            this->menuFSave->Name = L"menuFSave";
            this->menuFSave->ShortcutKeyDisplayString = L"Ctrl-S";
            this->menuFSave->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::S));
            this->menuFSave->Size = System::Drawing::Size(135, 22);
            this->menuFSave->Text = L"&Save";
            this->menuFSave->Click += gcnew System::EventHandler(this, &Document::menuFSave_Click);
            // 
            // menuFSaveAs
            // 
            this->menuFSaveAs->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"menuFSaveAs.Image")));
            this->menuFSaveAs->MergeAction = System::Windows::Forms::MergeAction::Insert;
            this->menuFSaveAs->MergeIndex = 3;
            this->menuFSaveAs->Name = L"menuFSaveAs";
            this->menuFSaveAs->Size = System::Drawing::Size(135, 22);
            this->menuFSaveAs->Text = L"Save &As";
            this->menuFSaveAs->Click += gcnew System::EventHandler(this, &Document::menuFSaveAs_Click);
            // 
            // menuFClose
            // 
            this->menuFClose->MergeAction = System::Windows::Forms::MergeAction::Insert;
            this->menuFClose->MergeIndex = 4;
            this->menuFClose->Name = L"menuFClose";
            this->menuFClose->Size = System::Drawing::Size(135, 22);
            this->menuFClose->Text = L"&Close";
            this->menuFClose->Click += gcnew System::EventHandler(this, &Document::menuFClose_Click);
            // 
            // menuFRevert
            // 
            this->menuFRevert->MergeAction = System::Windows::Forms::MergeAction::Insert;
            this->menuFRevert->MergeIndex = 5;
            this->menuFRevert->Name = L"menuFRevert";
            this->menuFRevert->Size = System::Drawing::Size(135, 22);
            this->menuFRevert->Text = L"Re&vert";
            this->menuFRevert->Click += gcnew System::EventHandler(this, &Document::menuFRevert_Click);
            // 
            // menuEdit
            // 
            this->menuEdit->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(10) {this->menuEUndo, 
                this->menuERedo, this->toolStripSeparator5, this->menuECut, this->menuECopy, this->menuEPaste, this->menuEDelete, this->toolStripSeparator6, 
                this->menuEFont, this->menuEFind});
            this->menuEdit->MergeAction = System::Windows::Forms::MergeAction::Insert;
            this->menuEdit->MergeIndex = 1;
            this->menuEdit->Name = L"menuEdit";
            this->menuEdit->Size = System::Drawing::Size(39, 20);
            this->menuEdit->Text = L"&Edit";
            // 
            // menuEUndo
            // 
            this->menuEUndo->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"menuEUndo.Image")));
            this->menuEUndo->Name = L"menuEUndo";
            this->menuEUndo->ShortcutKeyDisplayString = L"Ctrl-Z";
            this->menuEUndo->Size = System::Drawing::Size(189, 22);
            this->menuEUndo->Text = L"&Undo";
            this->menuEUndo->Click += gcnew System::EventHandler(this, &Document::menuEUndo_Click);
            // 
            // menuERedo
            // 
            this->menuERedo->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"menuERedo.Image")));
            this->menuERedo->Name = L"menuERedo";
            this->menuERedo->ShortcutKeyDisplayString = L"Ctrl-Y";
            this->menuERedo->Size = System::Drawing::Size(189, 22);
            this->menuERedo->Text = L"&Redo";
            this->menuERedo->Click += gcnew System::EventHandler(this, &Document::menuERedo_Click);
            // 
            // toolStripSeparator5
            // 
            this->toolStripSeparator5->Name = L"toolStripSeparator5";
            this->toolStripSeparator5->Size = System::Drawing::Size(186, 6);
            // 
            // menuECut
            // 
            this->menuECut->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"menuECut.Image")));
            this->menuECut->Name = L"menuECut";
            this->menuECut->ShortcutKeyDisplayString = L"Ctrl-X";
            this->menuECut->Size = System::Drawing::Size(189, 22);
            this->menuECut->Text = L"Cu&t";
            this->menuECut->Click += gcnew System::EventHandler(this, &Document::menuECut_Click);
            // 
            // menuECopy
            // 
            this->menuECopy->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"menuECopy.Image")));
            this->menuECopy->Name = L"menuECopy";
            this->menuECopy->ShortcutKeyDisplayString = L"Ctrl-C";
            this->menuECopy->Size = System::Drawing::Size(189, 22);
            this->menuECopy->Text = L"&Copy";
            this->menuECopy->Click += gcnew System::EventHandler(this, &Document::menuECopy_Click);
            // 
            // menuEPaste
            // 
            this->menuEPaste->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"menuEPaste.Image")));
            this->menuEPaste->Name = L"menuEPaste";
            this->menuEPaste->ShortcutKeyDisplayString = L"Ctrl-V";
            this->menuEPaste->Size = System::Drawing::Size(189, 22);
            this->menuEPaste->Text = L"&Paste";
            this->menuEPaste->Click += gcnew System::EventHandler(this, &Document::menuEPaste_Click);
            // 
            // menuEDelete
            // 
            this->menuEDelete->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"menuEDelete.Image")));
            this->menuEDelete->Name = L"menuEDelete";
            this->menuEDelete->ShortcutKeyDisplayString = L"DEL";
            this->menuEDelete->Size = System::Drawing::Size(189, 22);
            this->menuEDelete->Text = L"&Delete";
            this->menuEDelete->Click += gcnew System::EventHandler(this, &Document::menuEDelete_Click);
            // 
            // toolStripSeparator6
            // 
            this->toolStripSeparator6->Name = L"toolStripSeparator6";
            this->toolStripSeparator6->Size = System::Drawing::Size(186, 6);
            // 
            // menuEFont
            // 
            this->menuEFont->Name = L"menuEFont";
            this->menuEFont->Size = System::Drawing::Size(189, 22);
            this->menuEFont->Text = L"Set Font...";
            this->menuEFont->Click += gcnew System::EventHandler(this, &Document::menuEFont_Click);
            // 
            // menuEFind
            // 
            this->menuEFind->Name = L"menuEFind";
            this->menuEFind->ShortcutKeyDisplayString = L"Ctrl-F";
            this->menuEFind->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::F));
            this->menuEFind->Size = System::Drawing::Size(189, 22);
            this->menuEFind->Text = L"&Find/Replace...";
            // 
            // menuRender
            // 
            this->menuRender->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(10) {this->menuRRender, 
                this->menuRRenderSize, this->menuRRenderAgain, this->menuRStop, this->toolStripSeparator3, this->menuRImage, this->menuRMovie, 
                this->menuRUpload, this->toolStripSeparator4, this->menuRLegacy});
            this->menuRender->MergeAction = System::Windows::Forms::MergeAction::Insert;
            this->menuRender->MergeIndex = 2;
            this->menuRender->Name = L"menuRender";
            this->menuRender->Size = System::Drawing::Size(56, 20);
            this->menuRender->Text = L"&Render";
            // 
            // menuRRender
            // 
            this->menuRRender->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"menuRRender.Image")));
            this->menuRRender->Name = L"menuRRender";
            this->menuRRender->ShortcutKeyDisplayString = L"Ctrl-R";
            this->menuRRender->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::R));
            this->menuRRender->Size = System::Drawing::Size(220, 22);
            this->menuRRender->Text = L"&Render";
            this->menuRRender->Click += gcnew System::EventHandler(this, &Document::menuRRender_Click);
            // 
            // menuRRenderSize
            // 
            this->menuRRenderSize->Name = L"menuRRenderSize";
            this->menuRRenderSize->ShortcutKeyDisplayString = L"Ctrl-Shft-R";
            this->menuRRenderSize->ShortcutKeys = static_cast<System::Windows::Forms::Keys>(((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Shift) 
                | System::Windows::Forms::Keys::R));
            this->menuRRenderSize->Size = System::Drawing::Size(220, 22);
            this->menuRRenderSize->Text = L"Render to si&ze...";
            this->menuRRenderSize->Click += gcnew System::EventHandler(this, &Document::menuRRenderSize_Click);
            // 
            // menuRRenderAgain
            // 
            this->menuRRenderAgain->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"menuRRenderAgain.Image")));
            this->menuRRenderAgain->Name = L"menuRRenderAgain";
            this->menuRRenderAgain->ShortcutKeyDisplayString = L"Ctrl-Alt-R";
            this->menuRRenderAgain->ShortcutKeys = static_cast<System::Windows::Forms::Keys>(((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Alt) 
                | System::Windows::Forms::Keys::R));
            this->menuRRenderAgain->Size = System::Drawing::Size(220, 22);
            this->menuRRenderAgain->Text = L"Repea&t render";
            this->menuRRenderAgain->Click += gcnew System::EventHandler(this, &Document::menuRRenderAgain_Click);
            // 
            // menuRStop
            // 
            this->menuRStop->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"menuRStop.Image")));
            this->menuRStop->Name = L"menuRStop";
            this->menuRStop->ShortcutKeyDisplayString = L"Ctrl-END";
            this->menuRStop->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::End));
            this->menuRStop->Size = System::Drawing::Size(220, 22);
            this->menuRStop->Text = L"&Stop";
            this->menuRStop->Click += gcnew System::EventHandler(this, &Document::menuRStop_Click);
            // 
            // toolStripSeparator3
            // 
            this->toolStripSeparator3->Name = L"toolStripSeparator3";
            this->toolStripSeparator3->Size = System::Drawing::Size(217, 6);
            // 
            // menuRImage
            // 
            this->menuRImage->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"menuRImage.Image")));
            this->menuRImage->Name = L"menuRImage";
            this->menuRImage->ShortcutKeyDisplayString = L"Ctrl-I";
            this->menuRImage->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::I));
            this->menuRImage->Size = System::Drawing::Size(220, 22);
            this->menuRImage->Text = L"Save &image...";
            this->menuRImage->Click += gcnew System::EventHandler(this, &Document::menuRImage_Click);
            // 
            // menuRMovie
            // 
            this->menuRMovie->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"menuRMovie.Image")));
            this->menuRMovie->Name = L"menuRMovie";
            this->menuRMovie->ShortcutKeyDisplayString = L"Ctrl-M";
            this->menuRMovie->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::M));
            this->menuRMovie->Size = System::Drawing::Size(220, 22);
            this->menuRMovie->Text = L"Save &movie...";
            this->menuRMovie->Click += gcnew System::EventHandler(this, &Document::menuRMovie_Click);
            // 
            // menuRUpload
            // 
            this->menuRUpload->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"menuRUpload.Image")));
            this->menuRUpload->Name = L"menuRUpload";
            this->menuRUpload->ShortcutKeyDisplayString = L"Ctrl-U";
            this->menuRUpload->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::U));
            this->menuRUpload->Size = System::Drawing::Size(220, 22);
            this->menuRUpload->Text = L"&Upload to gallery...";
            this->menuRUpload->Click += gcnew System::EventHandler(this, &Document::menuRUpload_Click);
            // 
            // toolStripSeparator4
            // 
            this->toolStripSeparator4->Name = L"toolStripSeparator4";
            this->toolStripSeparator4->Size = System::Drawing::Size(217, 6);
            // 
            // menuRLegacy
            // 
            this->menuRLegacy->Name = L"menuRLegacy";
            this->menuRLegacy->Size = System::Drawing::Size(220, 22);
            this->menuRLegacy->Text = L"&Legacy variations";
            this->menuRLegacy->Click += gcnew System::EventHandler(this, &Document::menuRLegacy_Click);
            // 
            // Document
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(820, 404);
            this->Controls->Add(this->documentSplitter);
            this->Controls->Add(this->toolStrip1);
            this->Controls->Add(this->menuStrip1);
            this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
            this->Name = L"Document";
            this->TabText = L"Document";
            this->Text = L"Document";
            this->toolStrip1->ResumeLayout(false);
            this->toolStrip1->PerformLayout();
            this->documentSplitter->Panel1->ResumeLayout(false);
            this->documentSplitter->Panel2->ResumeLayout(false);
            this->documentSplitter->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->renderBox))->EndInit();
            this->menuStrip1->ResumeLayout(false);
            this->menuStrip1->PerformLayout();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
private: 
    System::Void RenderButton_Click(System::Object^  sender, System::EventArgs^  e);

    System::Void PrevVar_Click(System::Object^  sender, System::EventArgs^  e);

    System::Void NextVar_Click(System::Object^  sender, System::EventArgs^  e);

    System::Void menuFSave_Click(System::Object^  sender, System::EventArgs^  e);

    System::Void menuFSaveAs_Click(System::Object^  sender, System::EventArgs^  e);

    System::Void menuFClose_Click(System::Object^  sender, System::EventArgs^  e);

    System::Void menuFRevert_Click(System::Object^  sender, System::EventArgs^  e);

    System::Void menuRRender_Click(System::Object^  sender, System::EventArgs^  e);

    System::Void menuRRenderSize_Click(System::Object^  sender, System::EventArgs^  e);

    System::Void menuRRenderAgain_Click(System::Object^  sender, System::EventArgs^  e);

    System::Void menuRStop_Click(System::Object^  sender, System::EventArgs^  e);

    System::Void menuRImage_Click(System::Object^  sender, System::EventArgs^  e);

    System::Void menuRMovie_Click(System::Object^  sender, System::EventArgs^  e);

    System::Void menuRUpload_Click(System::Object^  sender, System::EventArgs^  e);

    System::Void menuRLegacy_Click(System::Object^  sender, System::EventArgs^  e);

    System::Void modifiedCFDG(System::Object^  sender, System::EventArgs^  e);

    System::Void stripSizeChanged(System::Object^ sender, System::EventArgs^ e);

    System::Void renderSizeChanging(System::Object^ sender, System::Windows::Forms::LayoutEventArgs^ e);

    System::Void variationKeyDown(Object^ , System::Windows::Forms::KeyEventArgs^ e);

    System::Void variationKeyPress(Object^ , System::Windows::Forms::KeyPressEventArgs^);

    System::Void variationChanged(System::Object^ sender, System::EventArgs^ e);

    void InitializeStuff();
    void DestroyStuff();
    System::Void moreInitialization(System::Object^ sender, System::EventArgs^ e);
    System::EventHandler^ moreInitHandler;

protected: 
    System::Void FormIsClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);
    virtual void WndProc(System::Windows::Forms::Message% m) override;
    void RunRenderThread(System::Object^, System::ComponentModel::DoWorkEventArgs^);
    void RenderCompleted(System::Object^, System::ComponentModel::RunWorkerCompletedEventArgs^);
    void DoRender();
    void updateRenderButton();
    int mRenderButtonIndex;
    bool SyncToSystem();
    System::Drawing::Bitmap^ MakeBitmap(bool cropped, WinCanvas* canvas);
    void renderSizeChanged();
    void setupCanvas();
    void updateRenderBox();
    int mProgressDelay;
    int mResizeDelay;
    bool mUserChangedVariation;
    int imageListBase;

    CFDG* mEngine;
    Renderer* mRenderer;
    WinCanvas* mCanvas;
    tiledCanvas* mTiledCanvas;
    SVGCanvas* mSVGCanvas;
    qtCanvas* mAnimationCanvas;
    bool mTiled;
    bool mRectangular;
    double mScale;
    agg::trans_affine* mTileTransform;
    System::Drawing::Size mRectangularSize;
    System::Drawing::Bitmap^ displayImage;
    System::Windows::Forms::Timer^ statusTimer;
    System::Void statusTick(System::Object^ sender, System::EventArgs^ e);
    System::Windows::Forms::Timer^ resizeTimer;
    System::Void resizeTick(System::Object^ sender, System::EventArgs^ e);

    System::EventHandler^ activateHandler;

private: System::Void menuEUndo_Click(System::Object^ sender, System::EventArgs^ e);

private: System::Void menuERedo_Click(System::Object^ sender, System::EventArgs^ e);

private: System::Void menuECut_Click(System::Object^  sender, System::EventArgs^  e);

private: System::Void menuECopy_Click(System::Object^  sender, System::EventArgs^  e);

private: System::Void menuEPaste_Click(System::Object^  sender, System::EventArgs^  e);

private: System::Void menuFile_Popup(System::Object^  sender, System::EventArgs^  e);

private: System::Void menuEdit_Popup(System::Object^  sender, System::EventArgs^  e);

private: System::Void menuEDelete_Click(System::Object^  sender, System::EventArgs^  e);

public: bool saveToPNGorJPEG(System::String^ path, System::IO::Stream^ str, 
             bool JPEG, bool rect);
private: void saveToSVG(System::String^ path);
private: void drawTiled(System::Drawing::Bitmap^ src, System::Drawing::Bitmap^ dest,
             System::Drawing::Graphics^ g, int x, int y);

private: System::Void renderBox_Click(System::Object^  sender, System::EventArgs^  e) {
             cfdgText->Focus();
         }
private: System::Void menuEFont_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void textFontHandler(System::Object^ sender, System::EventArgs^ e);
private: System::Void applyFontHandler(System::Object^ sender, System::EventArgs^ e);
private: System::Void splitterMoved(System::Object^ sender, System::Windows::Forms::SplitterEventArgs^ e);
private: System::EventHandler^ processVariationChange;
};
}
