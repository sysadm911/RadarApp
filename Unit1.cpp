// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

#define Pi M_PI
HWND hwnd;
HDC dc;
HGLRC hrc;
GLint viewport[4];
GLdouble modelMatrix[16], projMatrix[16];
GLfloat MCGrid[4];
GLdouble MCPoint[4];
GLdouble MCTarget[4];
GLdouble tX, tY, DX, DY, Zoom;
GLdouble D, Az, objx, objy, objz = 0;
GLdouble pPoints[2][2] = {0};

int click_counter;
bool allowPaintPoint = false;

// ---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::SetDCPixelFormat(HDC hdc) {
	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormat = 0;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_GENERIC_ACCELERATED | PFD_SUPPORT_GDI;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;
	nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, nPixelFormat, &pfd);
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender) {
	hwnd = Panel1->Handle;
	dc = GetDC(hwnd);
	SetDCPixelFormat(dc);
	hrc = wglCreateContext(dc);
	wglMakeCurrent(dc, hrc);

	glEnable(GL_DEPTH_TEST); // ��������� ���� �������
	glEnable(GL_LIGHTING); // ��������� ������ � �������������
	glEnable(GL_LIGHT0); // �������� �������� ����� 0

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	Radar.WaveLength = 100;
	Radar.I_F = 4794680;
	Radar.ADCClock = 19178720;
	Radar.PulseLength = 1668;
	Radar.wDN = 2;
	Radar.ADCWidth = 8;
	Radar.Noise = -60;
	Radar.PackRec[0].Period = 890987;
	Radar.PackRec[0].CntIZ = 16;
	Radar.PackRec[0].CntDskr = 509;
	Radar.PackRec[0].EnumKpp = 3;
	Radar.PackRec[1].Period = 954391;
	Radar.PackRec[1].CntIZ = 15;
	Radar.PackRec[1].CntDskr = 547;
	Radar.PackRec[1].EnumKpp = 2;
	Radar.PackRec[2].Period = 1027805;
	Radar.PackRec[2].CntIZ = 14;
	Radar.PackRec[2].CntDskr = 591;
	Radar.PackRec[2].EnumKpp = 1;
	Radar.PackRec[3].Period = 1214679;
	Radar.PackRec[3].CntIZ = 13;
	Radar.PackRec[3].CntDskr = 703;
	Radar.PackRec[3].EnumKpp = 0;
	currentTarget.Name = "Mig-29";
	currentTarget.S = 10;
	// currentTarget.Velocity = StrToFloat(edt_speed->Text);
	currentTarget.Distance = 50000;
	currentTarget.Beta = 90;
	currentTarget.Length = 10;
	currentTarget.Height = 1000;

	ListTr = new TList;

}

// ---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender) {
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hrc);
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::WMPaint(TWMPaint Msg) {

	TForm::Dispatch(&Msg);
	TPaintStruct ps;

	BeginPaint(hwnd, &ps);

	unsigned int Cl = RGB(55, 53, 74);
	glClearColor(double(LOBYTE(Cl)) / 255, double(HIBYTE(Cl)) / 255, double(LOBYTE(HIWORD(Cl))) / 255, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	PaintGrid();

	if (allowPaintPoint == true)
		PaintPoint();

	SwapBuffers(dc);
	// wglMakeCurrent(0, 0);
	// wglDeleteContext(hrc);

	EndPaint(hwnd, &ps);

}

// ---------------------------------------------------------------------------
void TForm1::PaintGrid() {
	// grid_list = glGenLists(1);
	// glNewList(grid_list, GL_COMPILE);
	const int nSec = 40;
	int i = 0, j = 0;
	float a = 0.0, x = 0.0, y = 0.0, k = 0.0, Cl = 0.0, ClMax = 0.0, ClMin = 0.0;
	int n = 0;
	n = 100;
	glLineWidth(1);
	MCGrid[0] = 1;
	MCGrid[1] = 1;
	MCGrid[2] = 1;
	MCGrid[3] = 1;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MCGrid);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);

	k = 2;

	Cl = 0.15 * k; // * FGridOpacity;
	ClMax = 0.25 * k; // * FGridOpacity;
	ClMin = 0.03 * k; // * FGridOpacity;
	for (int j = 0; j <= nSec; j++) {
		if (Frac(j / 5.0) == 0)
			MCGrid[3] = Cl;
		else
			MCGrid[3] = ClMin;
		if (Frac(double(j) / 10) == 0)
			MCGrid[3] = ClMax;
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MCGrid);

		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < n; i++) {
			a = i * 2 * Pi / n;
			x = j * sin(a) / nSec;
			y = j * cos(a) / nSec;
			glVertex3f(x, y, 0.01);
		}
		a = 1 * 2 * Pi / n;
		x = j * sin(a) / nSec;
		y = j * cos(a) / nSec;
		glVertex3f(x, y, 0.01);
		glEnd();
	}
	j = 60;
	for (int i = 0; i <= j; i++) {
		if (Frac(i / 5.0) == 0)
			MCGrid[3] = Cl;
		else
			MCGrid[3] = ClMin;
		if (Frac(double(i) / 15) == 0)
			MCGrid[3] = ClMax;
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MCGrid);
		glBegin(GL_LINE_STRIP);
		a = 2 * Pi * i / j;
		x = sin(a);
		y = cos(a);
		glVertex3f(x * 6. / 450, y * 6. / 450, 0.02);
		glVertex3f(x, y, 0.02);

		glEnd();

	}
	// glEndList();

	if (D < 451) {
		glPointSize(Zoom*2);
		glEnable(GL_POINT_SMOOTH);
		glColor3f(1.0, 1.0, 1.0);
		// glColor3f(Color2RGB(clyellow).R,Color2RGB(clyellow).g,Color2RGB(clyellow).b);
		glBegin(GL_POINTS);
		glVertex2f(objx, objy);
		glEnd();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Panel1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y) {
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	gluUnProject(X, (float)viewport[3] - Y, 0, modelMatrix, projMatrix, viewport, &objx, &objy, &objz);

	D = sqrt(objy * objy + objx * objx) * 450;
	// Az = ArcTan2(objy, objx);
	Az = ArcTan2(objx, objy);

	if (Az > 0)
		Az = 180 / Pi * Az;
	else
		Az = 360 + (180 / Pi * Az);

	Caption = Format("%3.3f ��,  %3.0f", ARRAYOFCONST((D, Az))) + " \xB0";
	StatusBar1->SimpleText = Format("OpenGL X_coord = %.6f,  Y_coord = %.6f,  Z_coord = %.6f", ARRAYOFCONST((objx, objy, objz)));

}

// ---------------------------------------------------------------------------
void __fastcall TForm1::Panel1Click(TObject *Sender) {

	switch (click_counter) {
	case 1:
		allowPaintPoint = true;
		ValueListEditor3->Cells[1][1] = D;
		ValueListEditor3->Cells[2][2] = Az;
		pPoints[0][0] = objx;
		pPoints[0][1] = objy;
		break;
	case 2:
		allowPaintPoint = true;
		ValueListEditor3->Cells[1][3] = D;
		ValueListEditor3->Cells[2][4] = Az;
		pPoints[1][0] = objx;
		pPoints[1][1] = objy;
		click_counter=0;
		break;
	default:
		allowPaintPoint = false;
		click_counter = 0;
		InvalidateRect(Handle, 0, true);

	}
	InvalidateRect(Handle, 0, true);
		click_counter++;
}

// ---------------------------------------------------------------------------
void TForm1::PaintPoint() {

	glPointSize(10);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	for (int i = 0; i < click_counter; i++)
		glVertex2f(pPoints[i][0], pPoints[i][1]);
	glEnd();
}

// ---------------------------------------------------------------------------
TTreeNode * TForm1::AddNodeRot(int i) {
	TTreeNode * result;
	result = TV->Items->Add(NULL, " ������ �" + IntToStr(i + 1));
	result->SelectedIndex = 0;
	return result;
}

// ---------------------------------------------------------------------------
TTreeNode * TForm1::AddNodePacket(TTreeNode * Parent, int i) {
	TTreeNode * result;
	result = TV->Items->AddChild(Parent, " ����� �" + IntToStr(i + 1));
	result->SelectedIndex = 0;
	return result;
}

// ---------------------------------------------------------------------------
TTreeNode * TForm1::AddNodePackage(TTreeNode * Parent, int i) {
	TTreeNode * result;
	result = TV->Items->AddChild(Parent, " ����� �" + IntToStr(i + 1));
	result->SelectedIndex = 0;
	return result;
}

// ---------------------------------------------------------------------------
TTreeNode * TForm1::AddNodePulses(TTreeNode * Parent, int i) {
	TTreeNode * result;
	result = TV->Items->AddChild(Parent, " ������ �" + IntToStr(i + 1));
	result->SelectedIndex = 0;
	return result;
}

// ---------------------------------------------------------------------------

float TForm1::GetTimePacket() {
	float result = 0.0;
	int stop = GetCountPackages();
	for (int i = 0; i < stop; i++) {
		result = result + (Radar.PackRec[i].CntIZ) * (Radar.PackRec[i].Period) * 1E-9;
	}
	return result;
}

int TForm1::GetCountPackages() {
	int result = 0;
	result = 4; // Radar.PackRec.Length;
	return result;
}

float TForm1::GetTimePackage(int PackageID) {
	float result = 0.0;
	int i = 0;
	result = Radar.PackRec[PackageID].Period * Radar.PackRec[PackageID].CntIZ * 1E-9;
	return result;
}

int TForm1::GetCountPulsesOfPacket() {
	int result = 0;
	int i = 0;
	result = 0;
	int stop = GetCountPackages();
	for (int i = 0; i < stop; i++) {
		result = result + Radar.PackRec[i].CntIZ;
	}
	return result;
}

int TForm1::GetCountRotation() {
	int result = 0;
	result = 1 * 6;
	return result;
}

int TForm1::GetCountPacketOfRotaion() {
	int result = 0;
	result = int(60.0 / GetCountRotation() / GetTimePacket());
	return result;
}

float TForm1::GetAzimutOfTime(float Time) {
	float result = 0.0;
	result = Time * 6.0 / 60.0 * 360.0;

	return result;
}

float TForm1::PulseToTime(int PacketID, int PackageID, int PulseId) {
	float result = 0.0;
	int i = 0, Cnt = 0;
	result = PacketID;
	i = PackageID; // - 1;
	while (i > 0) {
		result = result + GetTimePackage(i);
		i--;
	}
	result = result + PulseId * Radar.PackRec[PackageID].Period * 1E-9;
	return result;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender) {
	double deltaTime = 0, deltaBeta = 0, deltaDistance = 0, D_start = 0, D_startX = 0, D_startY = 0, D_end = 0, D_endX = 0, D_endY = 0,
		Decart_Route_Start = 0, Decart_Route_End = 0, Decart_Route = 0;

	if ((edt_beta3->Text != "") && (edt_beta0->Text != "")) {

		int i, j, ii, irot;

		TTreeNode * Root, *NodeRot, *NodePacket, *NodePackage, *NodePulses, *PulsesData, *tmp, *tmp2;

		TV->Items->Clear();

		TV->Items->BeginUpdate();

		int PulsesOfPacket = GetCountPulsesOfPacket();
		int CntPack = GetCountPacketOfRotaion();
		TimeArr = new int[PulsesOfPacket];
		BetaArr = new float[PulsesOfPacket];
		int step = 0, currentPulse = 0;

		// currentTarget.Velocity = StrToFloat(edt_speed->Text) * 1000.0 / 3600.0;
		currentTarget.Distance = StrToFloat(edt_D0->Text) * 1000;
		currentTarget.Beta = StrToFloat(edt_beta0->Text);

		float t1 = StrToFloat(edt_beta0->Text);
		float t2 = StrToFloat(edt_beta3->Text);

		ShowMessage(FloatToStr(t1));
		ShowMessage(FloatToStr(t2));

		D_start = StrToFloat(edt_D0->Text) * 1000.0;
		D_end = StrToFloat(edt_D3->Text) * 1000.0;

		D_startX = D_start * sin(Pi * StrToFloat(edt_beta0->Text) / 180);
		D_startY = D_start * cos(Pi * StrToFloat(edt_beta0->Text) / 180);

		D_endX = D_end * sin(Pi * StrToFloat(edt_beta3->Text) / 180);
		D_endY = D_end * cos(Pi * StrToFloat(edt_beta3->Text) / 180);

		Decart_Route = sqrt((D_startX + D_endX) * (D_startX + D_endX) + (D_startY + D_endY) * (D_startY + D_endY));
		Decart_Route_End = Decart_Route;

		// if (D_start < D_end) {
		//
		// irot = 0;
		// currentPulse = 0;
		// while (D_start < D_end) {
		//
		// NodeRot = AddNodeRot(irot);
		// irot++;
		//
		// for (i = 0; i < CntPack; i++) {
		//
		// NodePacket = AddNodePacket(NodeRot, i);
		// for (j = 0; j < 4; j++) {
		//
		// NodePackage = AddNodePackage(NodePacket, j);
		// for (ii = 0; ii < Radar.PackRec[j].CntIZ; ii++) {
		// NodePulses = AddNodePulses(NodePackage, ii);
		//
		// tmpItem = new TtmpItemRec;
		// tmpItem->ZapID = ii;
		// tmpItem->PackID = i;
		// tmpItem->PckgID = j;
		// tmpItem->Time = PulseToTime(i, j, ii);
		// tmpItem->Beta = GetAzimutOfTime(tmpItem->Time) + currentTarget.Beta;
		// if (t1 < t2) {
		// if (!((tmpItem->Beta >= t1) && (tmpItem->Beta <= t2))) {
		// tmpItem->Beta = 0;
		//
		// }
		// }
		//
		// if (t1 > t2) {
		// if (!((tmpItem->Beta >= t2) && (tmpItem->Beta <= t1))) {
		// tmpItem->Beta = 0;
		//
		// }
		// }
		// tmpItem->TimeBack = tmpItem->Time + 2.0 * D_start / 3.0e8;
		// tmpItem->deltaTime = tmpItem->TimeBack - tmpItem->Time;
		// tmpItem->deltaDistance = currentTarget.Velocity * (tmpItem->deltaTime);
		// tmpItem->X = (D_start / 450000.0) * cos(Pi * tmpItem->Beta / 180.0);
		// tmpItem->Y = (D_start / 450000.0) * sin(Pi * tmpItem->Beta / 180.0);
		//
		// tmpItem->Parent = NULL;
		//
		// NodePulses->Data = tmpItem;
		//
		// step = step + Radar.PackRec[j].Period;
		// currentPulse++;
		//
		// D_start += tmpItem->deltaDistance;
		// }
		//
		// }
		// }
		// }
		// // UpdateListPulses();
		// ShowMessage("IZ count = " + FloatToStr(currentPulse));
		//
		// }
		//
		// D_start = StrToFloat(edt_D0->Text) * 1000.0;
		// D_end = StrToFloat(edt_D3->Text) * 1000.0;
		//
		// if (D_start > D_end) {
		//
		// irot = 0;
		// currentPulse = 0;
		// while (D_start > D_end) {
		//
		// NodeRot = AddNodeRot(irot);
		// irot++;
		//
		// for (i = 0; i < CntPack; i++) {
		//
		// NodePacket = AddNodePacket(NodeRot, i);
		// for (j = 0; j < 4; j++) {
		//
		// NodePackage = AddNodePackage(NodePacket, j);
		// for (ii = 0; ii < Radar.PackRec[j].CntIZ; ii++) {
		// NodePulses = AddNodePulses(NodePackage, ii);
		//
		// tmpItem = new TtmpItemRec;
		// tmpItem->ZapID = ii;
		// tmpItem->PackID = i;
		// tmpItem->PckgID = j;
		// tmpItem->Time = PulseToTime(i, j, ii);
		// tmpItem->Beta = GetAzimutOfTime(tmpItem->Time) + currentTarget.Beta;
		// // while (tmpItem->Beta >= 360)
		// // tmpItem->Beta = tmpItem->Beta - 360.0;
		// if (t1 < t2) {
		// if (!((tmpItem->Beta >= t1) && (tmpItem->Beta <= t2))) {
		// tmpItem->Beta = 0;
		//
		// }
		// }
		//
		// if (t1 > t2) {
		// if (!((tmpItem->Beta >= t2) && (tmpItem->Beta <= t1))) {
		// tmpItem->Beta = 0;
		//
		// }
		// }
		//
		// tmpItem->TimeBack = tmpItem->Time + 2.0 * D_start / 3.0e8;
		// tmpItem->deltaTime = tmpItem->TimeBack - tmpItem->Time;
		// tmpItem->deltaDistance = currentTarget.Velocity * (tmpItem->deltaTime);
		// tmpItem->X = (D_start / 450000.0) * cos(Pi * tmpItem->Beta / 180.0);
		// tmpItem->Y = (D_start / 450000.0) * sin(Pi * tmpItem->Beta / 180.0);
		//
		// tmpItem->Parent = NULL;
		//
		// NodePulses->Data = tmpItem;
		//
		// currentPulse++;
		//
		// D_start -= tmpItem->deltaDistance;
		// }
		//
		// }
		// }
		// }
		// // UpdateListPulses();
		// ShowMessage("IZ count = " + FloatToStr(currentPulse));
		//
		// }
		// TV->Items->EndUpdate();
		// }
		// Cursor = crDefault;
		//
		// delete[]TimeArr;
		// delete[]BetaArr;
	}
}
// ---------------------------------------------------------------------------
