// ---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>
#include <ComCtrls.hpp>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <Math.hpp>

// ---------------------------------------------------------------------------
typedef struct Target {
	String Name;
	float S;
	float Velocity;
	float Distance;
	float Beta;
	float Length;
	float Height;
	float X, Y;
	float Ampl;
	int Time;
} Target;

typedef struct TRecFreqPackage {
	int Period, CntIZ, CntDskr, EnumKpp;
} TRecFreqPackage;

struct TRecRadar {
	int WaveLength;
	int PulseLength;
	int Deviation;
	int I_F;
	float wDN;
	int ADCClock;
	int ADCWidth;
	int LFMlength;
	float Noise;
	TRecFreqPackage PackRec[4];
};

struct TtmpItemRec {
	double deltaTime, deltaDistance, Beta, Time, Ampl, TimeBack, X, Y;
	int ZapID, PackID, PckgID;
	void* BackItem, *Parent, *ArrData;
};

enum TLFMMode {
	lfmUp, lfmDown
};

// ---------------------------------------------------------------------------
class TForm1 : public TForm {
__published: // IDE-managed Components

	TPanel *Panel1;
	TValueListEditor *ValueListEditor1;
	TValueListEditor *ValueListEditor2;
	TStatusBar *StatusBar1;
	TTreeView *TreeView1;
	TValueListEditor *ValueListEditor3;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Panel1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall Panel1Click(TObject *Sender);

private: // User declarations
	void __fastcall SetDCPixelFormat(HDC hdc);
	void __fastcall WMPaint(TWMPaint Msg);
	BEGIN_MESSAGE_MAP VCL_MESSAGE_HANDLER(WM_PAINT, TWMPaint, WMPaint);
	END_MESSAGE_MAP(TForm);

public: // User declarations
	void PaintGrid();
	void PaintPoint();
	__fastcall TForm1(TComponent* Owner);

};

// ---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
// ---------------------------------------------------------------------------
#endif
