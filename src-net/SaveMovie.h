// SaveMovie.h
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

#include "CFPrefs.h"

class qtCanvas;

namespace ContextFreeNet {

	/// <summary>
	/// Summary for SaveMovie
	/// </summary>
	public ref class SaveMovie : public FileDialogExtenders::FileDialogControlBase
	{
	public:
		SaveMovie(qtCanvas* canvas, System::String^ file, System::String^ dir) 
            : mQTcanvas(canvas)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            fileName = file;
            fileDir = dir;
		}

    public:
        System::Windows::Forms::DialogResult ShowTheDialog(System::Windows::Forms::IWin32Window^ owner);
        
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
        virtual void OnPrepareMSDialog() override;		
		~SaveMovie()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::Button^  buttonMovieSettings;
             qtCanvas* mQTcanvas;
    protected: 

    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::TextBox^  lengthBox;
    private: System::Windows::Forms::ComboBox^  lengthType;
    public: System::Windows::Forms::CheckBox^  checkZoom;
    private: 




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
            this->buttonMovieSettings = (gcnew System::Windows::Forms::Button());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->lengthBox = (gcnew System::Windows::Forms::TextBox());
            this->lengthType = (gcnew System::Windows::Forms::ComboBox());
            this->checkZoom = (gcnew System::Windows::Forms::CheckBox());
            this->SuspendLayout();
            // 
            // buttonMovieSettings
            // 
            this->buttonMovieSettings->AutoSize = true;
            this->buttonMovieSettings->Location = System::Drawing::Point(115, 5);
            this->buttonMovieSettings->Name = L"buttonMovieSettings";
            this->buttonMovieSettings->Size = System::Drawing::Size(88, 23);
            this->buttonMovieSettings->TabIndex = 0;
            this->buttonMovieSettings->Text = L"Movie Settings";
            this->buttonMovieSettings->UseVisualStyleBackColor = true;
            this->buttonMovieSettings->Click += gcnew System::EventHandler(this, &SaveMovie::buttonMovieSettings_Click);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(209, 10);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(43, 13);
            this->label1->TabIndex = 1;
            this->label1->Text = L"Length:";
            // 
            // lengthBox
            // 
            this->lengthBox->Location = System::Drawing::Point(259, 7);
            this->lengthBox->Name = L"lengthBox";
            this->lengthBox->Size = System::Drawing::Size(60, 20);
            this->lengthBox->TabIndex = 2;
            // 
            // lengthType
            // 
            this->lengthType->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->lengthType->FormattingEnabled = true;
            this->lengthType->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"Seconds", L"Frames"});
            this->lengthType->Location = System::Drawing::Point(326, 6);
            this->lengthType->Name = L"lengthType";
            this->lengthType->Size = System::Drawing::Size(81, 21);
            this->lengthType->TabIndex = 3;
            // 
            // checkZoom
            // 
            this->checkZoom->AutoSize = true;
            this->checkZoom->Location = System::Drawing::Point(414, 10);
            this->checkZoom->Name = L"checkZoom";
            this->checkZoom->Size = System::Drawing::Size(53, 17);
            this->checkZoom->TabIndex = 4;
            this->checkZoom->Text = L"Zoom";
            this->checkZoom->UseVisualStyleBackColor = true;
            // 
            // SaveMovie
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->Controls->Add(this->checkZoom);
            this->Controls->Add(this->lengthType);
            this->Controls->Add(this->lengthBox);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->buttonMovieSettings);
            this->FileDlgCheckFileExists = false;
            this->FileDlgDefaultExt = L"mov";
            this->FileDlgFilter = L"Movie files (*.mov)|*.mov";
            this->FileDlgOkCaption = L"&Save";
            this->FileDlgStartLocation = FileDialogExtenders::AddonWindowLocation::Bottom;
            this->FileDlgType = Win32Types::FileDialogType::SaveFileDlg;
            this->Name = L"SaveMovie";
            this->Size = System::Drawing::Size(466, 37);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
        private:
            System::String^ fileName;
            System::String^ fileDir;
    private: System::Void buttonMovieSettings_Click(System::Object^  sender, System::EventArgs^  e);
};
}
