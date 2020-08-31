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

	// float tmpX, tmpY;
	//
	// if (Shift.Contains(ssRight)) {
	//
	// MousePosMove.X = MousePosMove.X + objx;
	// MousePosMove.Y = MousePosMove.Y + objy;
	// MousePosMove.State = siManualDrag;
	// InvalidateRect(hwnd1, 0, false);
	// }
	//
	// else
	// MousePosMove.State = siNone;
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::Panel1Click(TObject *Sender) {
	allowPaintPoint = true;
	ValueListEditor1->Values["D_start"]=D;

	InvalidateRect(Handle, 0, true);
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
void TForm1::PaintPoint() {
	glPointSize(10);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	glVertex2f(objx, objy);
	glEnd();
}
