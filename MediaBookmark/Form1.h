#pragma once


namespace MediaBookmark {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Windows::Media;
	using namespace System::IO;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		ref class Bookmark : public System::Windows::Forms::Panel {

		private:

			unsigned int index;
			System::Windows::Forms::Label^ title;
			System::Windows::Forms::Label^ xButton;
			System::Windows::Forms::TextBox^  annot;
			System::TimeSpan^ t;
			Form1^ parent;

		public:

			Bookmark(int index, Form1^ parent) : index(index), parent(parent) {
				InitializeThis();
			};

			void InitializeThis()
			{
				title = gcnew Label();
				xButton = gcnew Label();
				annot = gcnew TextBox();
				//
				//title
				//
				this->title->Font = (gcnew System::Drawing::Font(L"굴림", 10.F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(128)));
				this->title->ForeColor = System::Drawing::Color::Black;
				this->title->Name = L"label1";
				this->title->Location = System::Drawing::Point(5, 2);
				this->title->Click += gcnew System::EventHandler(this, &Form1::Bookmark::This_Clicked);
				//
				//xButton
				//
				this->xButton->Font = (gcnew System::Drawing::Font(L"굴림", 7.F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(128)));
				this->xButton->ForeColor = System::Drawing::Color::Gray;
				this->xButton->Name = L"label1";
				this->xButton->Text = L"X";
				this->xButton->Location = System::Drawing::Point(215, 2);
				this->xButton->Size = System::Drawing::Size(10, 10);
				this->xButton->Click += gcnew System::EventHandler(this, &Form1::Bookmark::XButton_Clicked);
				this->xButton->MouseEnter += gcnew System::EventHandler(this, &Form1::Bookmark::XButton_MouwEnter);
				this->xButton->MouseLeave += gcnew System::EventHandler(this, &Form1::Bookmark::XButton_MouwLeave);
				//
				//annot
				//
				this->annot->BackColor = System::Drawing::Color::AliceBlue;
				this->annot->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
				this->annot->Font = (gcnew System::Drawing::Font(L"굴림", 9.F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(128)));
				this->annot->ForeColor = System::Drawing::SystemColors::ControlDark;
				this->annot->Location = System::Drawing::Point(1, 25);
				this->annot->MaxLength = 1800;
				this->annot->Multiline = true;
				this->annot->Name = L"textBox1";
				this->annot->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
				this->annot->Size = System::Drawing::Size(210, 65);
				this->annot->GotFocus += gcnew System::EventHandler(this, &Form1::Bookmark::AnnotBox_Focused);
				this->annot->LostFocus += gcnew System::EventHandler(this, &Form1::Bookmark::AnnotBox_Unfocused);
				this->annot->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Bookmark::AnnotBox_KeyDown);
				//
				//Bookmark
				//
				this->BackColor = System::Drawing::Color::Aquamarine;
				this->Location = System::Drawing::Point(5, 2 + 103 * index);
				this->Name = L"bookmark" + index;
				this->Size = System::Drawing::Size(230, 100);
				this->Controls->Add(title);
				this->Controls->Add(xButton);
				this->Controls->Add(annot);
				this->Click += gcnew System::EventHandler(this, &Form1::Bookmark::This_Clicked);
				this->MouseLeave += gcnew System::EventHandler(this, &Form1::Bookmark::This_MouseHover);
			}

			void FromString(String^ bmStr)
			{
				String^ delim = "*|";
				array<wchar_t>^ delimch = delim->ToCharArray();
				auto tokens = bmStr->Split(delimch);

				int flag = 0;
				for each(auto it in tokens)
				{

					if (flag == 0)
					{
						t = TimeSpan::FromMilliseconds(Convert::ToDouble(it));
						flag++;
					}
					else
					{
						this->annot->Text = it;
					}
				}
				this->title->Text = String::Format("{0:D2}h:{1:D2}m:{2:D2}s", t->Hours, t->Minutes, t->Seconds);
			}

			void setTitle(String^ tStr)
			{
				this->title->Text = tStr;
			}

			void setTime(TimeSpan^ t)
			{
				this->t = t;
			}

			String^ ToString() override
			{
				return String::Format(t->TotalMilliseconds + "*" + annot->Text + "|");
			}

			void PointRefresh()
			{
				this->index = parent->bmArray->IndexOf(this);
				this->Location = System::Drawing::Point(5, 2 + 103 * index);
			}//Bookmark객체의 위치를 재조정한다.

			System::Void This_Clicked(System::Object^ sender, System::EventArgs^ e) {

				this->BackColor = System::Drawing::Color::YellowGreen;
				parent->_mp->Position = *t;
			}//북마크가 클릭되었을 떄

			System::Void XButton_Clicked(System::Object^ sender, System::EventArgs^ e) {

				ArrayList^ myFriends = parent->bmArray;
				myFriends->Remove(this);
				delete this;
				parent->PanelControls_FromArrList(parent->panel1, parent->bmArray); 
			}//북마크의 x표시가 클릭되었을 때

			System::Void XButton_MouwEnter(System::Object^ sender, System::EventArgs^ e) {

				this->xButton->BackColor = System::Drawing::Color::DeepSkyBlue;
			}//북마크의 x표시 위로 마우스가 올라갈 때

			System::Void XButton_MouwLeave(System::Object^ sender, System::EventArgs^ e) {

				this->xButton->ResetBackColor();
			}//북마크의 x표시에 있던 마우스가 떠나갈 때
			
			System::Void This_MouseHover(System::Object^ sender, System::EventArgs^ e) {

				this->BackColor = System::Drawing::Color::Aquamarine;
			}//마우스카 북마크를 떠날 때

			System::Void AnnotBox_Focused(System::Object^ sender, System::EventArgs^ e) {

				this->annot->ForeColor = System::Drawing::Color::DarkBlue;
			}//주석 박스가 포커스 당할 때

			System::Void AnnotBox_Unfocused(System::Object^ sender, System::EventArgs^ e) {

				this->annot->ForeColor = System::Drawing::SystemColors::ControlDark;
			}//주석 박스가 포커스 해제될 때

			System::Void AnnotBox_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {

				if (e->KeyCode == Keys::Escape)
					this->parent->button8->Focus();
			}
		}; // end Bookmark class

		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::Button^  button7;
	private: System::Windows::Forms::Button^  button8;
	private: System::Windows::Forms::Button^  button9;
	private: System::Windows::Forms::Button^  button10;
	private: System::Windows::Media::MediaPlayer^ _mp;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TrackBar^ trackBar1;
	private: System::Windows::Forms::Timer^  timer1;
	private: double MediaDuration;
	private: ArrayList^ bmArray = gcnew ArrayList();

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

		void PanelControls_FromArrList(System::Windows::Forms::Panel^ p, System::Collections::ArrayList^ arr)
		{
			p->Controls->Clear();
			IEnumerator^ i = arr->GetEnumerator();
			while (i->MoveNext())
			{
				static_cast<Bookmark^>(i->Current)->PointRefresh();
				p->Controls->Add(static_cast<Bookmark^>(i->Current));
			}

			for (register int i = 0; i < 3; i++)
				this->panel1->VerticalScroll->Value = this->panel1->VerticalScroll->Maximum;///
		}


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->button9 = (gcnew System::Windows::Forms::Button());
			this->button10 = (gcnew System::Windows::Forms::Button());
			this->_mp = (gcnew System::Windows::Media::MediaPlayer());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->timer1 = (gcnew System::Windows::Forms::Timer());
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->AutoScroll = true;
			this->panel1->BackColor = System::Drawing::Color::SeaShell;
			this->panel1->Location = System::Drawing::Point(439, 12);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(248, 319);
			this->panel1->TabIndex = 0;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(12, 12);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Media..";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(12, 308);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->BackColor = System::Drawing::Color::Ivory;
			this->button2->TabIndex = 1;
			this->button2->Text = L"▶";
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			this->button2->FlatStyle = FlatStyle::Flat;
			this->button2->FlatAppearance->BorderSize = 2;
			this->button2->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(93, 308);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->BackColor = System::Drawing::Color::Ivory;
			this->button3->TabIndex = 1;
			this->button3->Text = L"┃┃";
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			this->button3->FlatStyle = FlatStyle::Flat;
			this->button3->FlatAppearance->BorderSize = 2;
			this->button3->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(174, 308);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(75, 23);
			this->button4->BackColor = System::Drawing::Color::Ivory;
			this->button4->TabIndex = 1;
			this->button4->Text = L"■";
			this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
			this->button4->FlatStyle = FlatStyle::Flat;
			this->button4->FlatAppearance->BorderSize = 2;
			this->button4->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(255, 308);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(75, 23);
			this->button5->ForeColor = System::Drawing::Color::LightYellow;
			this->button5->BackColor = System::Drawing::Color::MidnightBlue;
			this->button5->TabIndex = 1;
			this->button5->Text = L"+10%";
			this->button5->Click += gcnew System::EventHandler(this, &Form1::button5_Click);
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(336, 308);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(75, 23);
			this->button6->ForeColor = System::Drawing::Color::LightYellow;
			this->button6->BackColor = System::Drawing::Color::OrangeRed;
			this->button6->TabIndex = 1;
			this->button6->Text = L"-10%";
			this->button6->Click += gcnew System::EventHandler(this, &Form1::button6_Click);
			// 
			// button7
			// 
			this->button7->Location = System::Drawing::Point(336, 12);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(75, 23);
			this->button7->BackColor = System::Drawing::Color::Gold;
			this->button7->FlatStyle = FlatStyle::Flat;
			this->button7->FlatAppearance->BorderColor = System::Drawing::Color::Yellow;
			this->button7->TabIndex = 1;
			this->button7->Text = L"☆ Add";
			this->button7->Click += gcnew System::EventHandler(this, &Form1::button7_Click);
			// 
			// button8
			// 
			this->button8->Location = System::Drawing::Point(336, 40);
			this->button8->Name = L"button7";
			this->button8->Size = System::Drawing::Size(75, 23);
			this->button8->TabIndex = 1;
			this->button8->Text = L"Save";
			this->button8->UseVisualStyleBackColor = true;
			this->button8->Click += gcnew System::EventHandler(this, &Form1::button8_Click);
			// 
			// button9
			//
			this->button9->Location = System::Drawing::Point(174, 280);
			this->button9->Name = L"button5";
			this->button9->Size = System::Drawing::Size(75, 23);
			this->button9->TabIndex = 1;
			this->button9->Text = L"◀◀";
			this->button9->UseVisualStyleBackColor = true;
			this->button9->Click += gcnew System::EventHandler(this, &Form1::button9_Click);
			// 
			// button10
			// 
			this->button10->Location = System::Drawing::Point(255, 280);
			this->button10->Name = L"button5";
			this->button10->Size = System::Drawing::Size(75, 23);
			this->button10->TabIndex = 1;
			this->button10->Text = L"▶▶";
			this->button10->UseVisualStyleBackColor = true;
			this->button10->Click += gcnew System::EventHandler(this, &Form1::button10_Click);
			//
			// _mp
			//
			this->_mp->Volume = 1;
			this->_mp->SpeedRatio = 1.;
			this->_mp->MediaOpened += gcnew System::EventHandler(this, &Form1::Media_Opened);
			//
			// label1
			//
			this->label1->Location = System::Drawing::Point(12, 38);
			this->label1->ForeColor = System::Drawing::Color::SeaShell;
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(400, 15);
			this->label1->Text = L"";
			//
			// label2
			//
			this->label2->Location = System::Drawing::Point(12, 291);
			this->label2->Name = L"label2";
			this->label2->ForeColor = System::Drawing::Color::SeaShell;
			this->label2->Size = System::Drawing::Size(75, 10);
			this->label2->Font = (gcnew System::Drawing::Font(L"굴림", 7.f));
			this->label2->Text = L"x1배속";
			//
			// label3
			//
			this->label3->Location = System::Drawing::Point(12, 280);
			this->label3->Name = L"label3";
			this->label3->ForeColor = System::Drawing::Color::SeaShell;
			this->label3->Size = System::Drawing::Size(75, 15);
			this->label3->Text = L"time :";
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(0, 240);
			this->trackBar1->ForeColor = System::Drawing::Color::SeaGreen;
			this->trackBar1->Maximum = 5400;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(425, 45);
			this->trackBar1->TabIndex = 4;
			this->trackBar1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::trackBar1_Clicked);
			this->trackBar1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::trackBar1_MouseUp);
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			this->timer1->Interval = 100;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(699, 343);
			this->MaximizeBox = false;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->BackColor = System::Drawing::Color::LightSlateGray;
			this->Controls->Add(this->button10);
			this->Controls->Add(this->button9);
			this->Controls->Add(this->button8);
			this->Controls->Add(this->button7);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->trackBar1);
			this->Name = L"Form1";
			this->Text = L"Media Bookmarker v2016.03.29";
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"icon")));
			this->ResumeLayout(false);
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
		}
#pragma endregion

	private : System::Void control_PreviewKeyDown(Object^ sender, PreviewKeyDownEventArgs^ e) {

		if (e->KeyCode == Keys::Up || e->KeyCode == Keys::Down || e->KeyCode == Keys::Left || e->KeyCode == Keys::Right || e->KeyCode == Keys::Tab)
		{
			if (e->KeyCode == Keys::Left)
				Form1::button9_Click(this, nullptr);

			else if (e->KeyCode == Keys::Right)
				Form1::button10_Click(this, nullptr);

			e->IsInputKey = true; //important!!
		}
		else if (e->KeyCode == Keys::Z)
			this->button6_Click(this, nullptr);
		else if (e->KeyCode == Keys::C)
			this->button5_Click(this, nullptr);
		else if (e->KeyCode == Keys::X)
		{
			this->_mp->SpeedRatio = 1.;
			this->label2->Text = "x1배속";
		}
	}//preview KeyEvent

	private: System::Void Form1_Load(System::Object^ sender, System::EventArgs^ e) {

		for each(Control^ control in this->Controls)
		{
			control->PreviewKeyDown += gcnew PreviewKeyDownEventHandler(this, &Form1::control_PreviewKeyDown);
		}
	}

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) { // the button for media loading

		//create an OFD instance.
		OpenFileDialog^ ofd = gcnew OpenFileDialog();
		ofd->AddExtension = true;
		ofd->DefaultExt = "*.*";
		ofd->Filter = "Media(*.*)|*.*";
		ofd->ShowDialog();

		try {
			_mp->Open(gcnew Uri(ofd->FileName));
		}
		catch (Exception^ e) {
			delete e;
			MessageBox::Show("파일 선택 없음. 또는 비지원 형식의 파일.");
		}
	}//end button1_Click

	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) { // play button

		_mp->Play();
	}//end button2_Click

	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) { // pause button

		_mp->Pause();
	}//end button3_Click

	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) { // stop button

		_mp->Stop();
	}//end button4_Click

	private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) { // speed up button

		_mp->SpeedRatio += 0.1;
		label2->Text = "x" + _mp->SpeedRatio + L"배속";
	}//end button5_Click

	private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) { // speed down button

		_mp->SpeedRatio -= 0.1;
		label2->Text = "x" + _mp->SpeedRatio + L"배속";
	}//end button6_Click

	private: System::Void button7_Click(System::Object^  sender, System::EventArgs^  e) { // bookmark button

		Bookmark^ bm = gcnew Bookmark(bmArray->Count, this);
		bm->setTitle(label3->Text);
		bm->setTime(_mp->Position);
		bmArray->Add(bm);

		this->panel1->VerticalScroll->Value = 0;
		this->panel1->Controls->Add(bm);
		for (register int i = 0; i < 3; i++)
			this->panel1->VerticalScroll->Value = this->panel1->VerticalScroll->Maximum;
	}//end button7_Click

	private: System::Void button8_Click(System::Object^  sender, System::EventArgs^  e) { // save button

		String^ originPath;
		StreamWriter^ sw;
		try {
			originPath = _mp->Source->AbsolutePath;
			sw = gcnew StreamWriter(originPath + ".bm");
		}
		catch (Exception^ e) {

			try {
				originPath = _mp->Source->LocalPath;
				sw = gcnew StreamWriter(originPath + ".bm");
			}
			catch (Exception^ le) {
				MessageBox::Show("실패: 미디어 파일을 먼저 불러오십시오.");
				return;
			}
		}

		String^ bm_fs;
		IEnumerator^ i = bmArray->GetEnumerator();
		while (i->MoveNext())
		{
			bm_fs = static_cast<Bookmark^>(i->Current)->ToString();
			sw->Write(bm_fs);
		}
		sw->Close();
		MessageBox::Show("Saved :)");
	}//end button8_Click

	private: System::Void button9_Click(System::Object^  sender, System::EventArgs^  e) { // move backward
		
		double modified_time = this->_mp->Position.TotalMilliseconds - 3000.;

		if (modified_time < 0)
			modified_time = 0;

		this->_mp->Position = TimeSpan::FromMilliseconds(modified_time);
	}//end button9_Click

	private: System::Void button10_Click(System::Object^  sender, System::EventArgs^  e) { // move forward

		double modified_time = this->_mp->Position.TotalMilliseconds + 3000.;

		if (modified_time > _mp->NaturalDuration.TimeSpan.TotalMilliseconds)
			modified_time = _mp->NaturalDuration.TimeSpan.TotalMilliseconds;

		this->_mp->Position = TimeSpan::FromMilliseconds(modified_time);
	}//end button10_Click

	private: System::Void Media_Opened(System::Object^ sender, System::EventArgs^ e) {

		//미디어 정보로부터 변수들 초기화
		try {
			array<String^>^ paths = _mp->Source->ToString()->Split(Path::DirectorySeparatorChar, Path::AltDirectorySeparatorChar);
			this->label1->Text = paths[paths->Length - 1];

			this->panel1->Controls->Clear();
			this->bmArray->Clear();

			this->trackBar1->Value = 0;
			timer1->Enabled = true;

			MediaDuration = _mp->NaturalDuration.TimeSpan.TotalMilliseconds;
		}
		catch (Exception^ e) {

			MessageBox::Show("Unexpected File or Action.");
			timer1->Enabled = false;
			MessageBox::Show(e->Message);
		}

		//bm파일 읽기 시도
		StreamReader^ sr;
		String^ buf;
		String^ delim = "|\0";
		try {
			sr = gcnew StreamReader(_mp->Source->AbsolutePath + ".bm");
		}
		catch (Exception^ e) {

			try {
				sr = gcnew StreamReader(_mp->Source->LocalPath + ".bm");
			}
			catch (Exception^ le) {
				MessageBox::Show("북마크 없음:\n" + le->Message);
			}
		}

		//bm파일 해석 시도
		try {
			if (sr && sr->BaseStream->CanRead)
			{
				buf = sr->ReadToEnd();
				auto results = buf->Split(delim->ToCharArray());
				for (int i = 0; i < results->Length - 1; i++)
				{
					Bookmark^ bm = gcnew Bookmark(bmArray->Count, this);
					bmArray->Add(bm);
					bm->FromString(Convert::ToString(results[i]));
					panel1->Controls->Add(bm);
				}
			}
		}
		catch (Exception^ e) {
			MessageBox::Show("올바른 북마크 파일이 아닙니다.:\n" + e->Message);
		}
		finally{
			if(sr)
				sr->Close();
		}
	}//end Media_Opened

	private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {

		TimeSpan^ t = _mp->Position;
		static int h, m, s;
		h = t->Hours, m = t->Minutes, s = t->Seconds;
		this->label3->Text = String::Format("{0:D2}h:{1:D2}m:{2:D2}s", h, m, s);

		double presentPos = t->TotalMilliseconds;
		if (presentPos <= MediaDuration)
			this->trackBar1->Value = 5400 * presentPos / MediaDuration;
	}//end timer1_tick

	private: System::Void trackBar1_Clicked(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {

		if (timer1->Enabled)
			timer1->Enabled = false;
		_mp->Pause();
	}//end trackBar1_Clicked

	private: System::Void trackBar1_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {

		double newPos = trackBar1->Value * (MediaDuration / 5400.);
		_mp->Position = TimeSpan::FromMilliseconds(newPos);
		_mp->Play();
		timer1->Enabled = true;
	}//end trackBar1_MouseUp
	};//end Form1 class
}//end MediaBookmark namespace
/*20160328 13시 ~ 24시, 705Lines*/