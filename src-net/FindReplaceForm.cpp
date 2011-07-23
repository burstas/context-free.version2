// FindReplaceForm.cpp
// this file is part of Context Free
// ---------------------
// Copyright (C) 2010 John Horigan - john@glyphic.com
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
#include "FindReplaceForm.h"
#include "Document.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace WeifenLuo::WinFormsUI::Docking;

namespace ContextFreeNet {
    System::Void FindReplaceForm::findText_Changed(System::Object^  sender, System::EventArgs^  e)
    {
        bool canFind = findText->Text->Length > 0;
        findButton->Enabled = canFind;
        replaceButton->Enabled = false;
        replaceAllButton->Enabled = canFind;
        replaceFindButton->Enabled = false;
    }

    System::Void FindReplaceForm::find_Click(System::Object^  sender, System::EventArgs^  e)
    {
        RichTextBox^ haystack = getRTB();
        if (findText->Text->Length == 0 || haystack == nullptr) {
            System::Media::SystemSounds::Beep->Play();
            return;
        }

        doFind(haystack, false);
        haystack->Focus();
    }

    System::Void FindReplaceForm::replace_Click(System::Object^  sender, System::EventArgs^  e)
    {
        RichTextBox^ haystack = doReplace(nullptr);
        if (haystack != nullptr) haystack->Focus();
    }

    System::Void FindReplaceForm::replaceAll_Click(System::Object^  sender, System::EventArgs^  e)
    {
        RichTextBox^ haystack = getRTB();
        if (findText->Text->Length == 0 || haystack == nullptr) {
            System::Media::SystemSounds::Beep->Play();
            return;
        }

        haystack->SelectionStart = 0;
        haystack->SelectionLength = 0;

        if (!doFind(haystack, true)) {
            notFound->Visible = true;
            System::Media::SystemSounds::Beep->Play();
            haystack->SelectionStart = haystack->Text->Length;
            haystack->SelectionLength = 0;
            haystack->Focus();
            return;
        }

        do {
            doReplace(haystack);
        } while (doFind(haystack, true));
        haystack->Focus();
    }

    System::Void FindReplaceForm::replaceFind_Click(System::Object^  sender, System::EventArgs^  e)
    {
        RichTextBox^ haystack = doReplace(nullptr);
        if (haystack != nullptr) {
            doFind(haystack, false);
            haystack->Focus();
        }
    }

    bool FindReplaceForm::doFind(RichTextBox^ haystack, bool allMode)
    {
        bool found = false;
        if (haystack == nullptr) return false;

        bool up = !allMode && upwardCheck->Checked;
        bool whole = wholeCheck->Checked;
        bool ic = ignoreCaseCheck->Checked;
        bool wrap = !allMode && wrapAroundCheck->Checked;
        RichTextBoxFinds finds = 
            (up ? RichTextBoxFinds::Reverse : RichTextBoxFinds::None) |
            (whole ? RichTextBoxFinds::WholeWord : RichTextBoxFinds::None) |
            (ic ? RichTextBoxFinds::None : RichTextBoxFinds::MatchCase);
        int at = haystack->SelectionStart;
        if (!up) at += haystack->SelectionLength;

        int needle = up ? haystack->Find(findText->Text, 0, at, finds) : 
            haystack->Find(findText->Text, at, -1, finds);

        found = needle >= 0;
        if (!found && wrap) {
            needle = up ? haystack->Find(findText->Text, at, -1, finds) : 
                needle = haystack->Find(findText->Text, 0, at, finds);
            found = needle >= 0;
        }

        replaceButton->Enabled = found;
        replaceFindButton->Enabled = found;
        notFound->Visible = !found && !allMode;
        if (!found && !allMode) System::Media::SystemSounds::Beep->Play();

        return found;
    }

    System::Windows::Forms::RichTextBox^ FindReplaceForm::doReplace(System::Windows::Forms::RichTextBox^ rtb)
    {
        RichTextBox^ haystack = rtb;
        if (haystack == nullptr) haystack = getRTB();
        if (findText->Text->Length == 0 || haystack == nullptr ||
            String::Compare(haystack->SelectedText, findText->Text, ignoreCaseCheck->Checked) != 0)
        {
            System::Media::SystemSounds::Beep->Play();
            return nullptr;
        }

        haystack->SelectedText = replaceText->Text;

        if (!upwardCheck->Checked)
            haystack->SelectionStart += haystack->SelectionLength;
        haystack->SelectionLength = 0;

        return haystack;
    }

    System::Windows::Forms::RichTextBox^ FindReplaceForm::getRTB()
    {
        Document^ cfdg = dynamic_cast<Document^>(DockPanel->ActiveDocument);
        return (cfdg != nullptr) ? cfdg->cfdgText : nullptr;
    }
}