// Authors: Jan Musinsky (jan.musinsky@cern.ch)
//          Martin Vala  (martin.vala@cern.ch)
// Date:    12 Mar 2013

#include <TCanvas.h>
#include <TGClient.h>
#include <TQObject.h>
#include <TGraphErrors.h>
#include <TGraphPainter.h>
#include <TPad.h>
#include <TLatex.h>
#include <TROOT.h>
#include <TDatime.h>
#include <TVirtualPad.h>
#include <TMarker.h>
#include <TAttMarker.h>

#include "AliROMGraphErrors.h"

ClassImp(AliROMGraphErrors)

//______________________________________________________________________________
AliROMGraphErrors::AliROMGraphErrors()
: TGraphErrors(),
  fFlashPoint(0),
  fFlashMarker(0)
{
  // AliROMGraphErrors default constructor
}
//______________________________________________________________________________
AliROMGraphErrors::AliROMGraphErrors(Int_t n)
: TGraphErrors(n),
  fFlashPoint(0),
  fFlashMarker(0)
{
  // AliROMGraphErrors normal constructor
}
//______________________________________________________________________________
AliROMGraphErrors::AliROMGraphErrors(Int_t n, const Float_t *x, const Float_t *y,
                                     const Float_t *ex, const Float_t *ey)
: TGraphErrors(n, x, y, ex, ey),
  fFlashPoint(0),
  fFlashMarker(0)
{
  // AliROMGraphErrors normal constructor
}
//______________________________________________________________________________
AliROMGraphErrors::AliROMGraphErrors(Int_t n, const Double_t *x, const Double_t *y,
                                     const Double_t *ex, const Double_t *ey)
: TGraphErrors(n, x, y, ex, ey),
  fFlashPoint(0),
  fFlashMarker(0)
{
  // AliROMGraphErrors normal constructor
}
//______________________________________________________________________________
AliROMGraphErrors::AliROMGraphErrors(const char *filename, const char *format, Option_t *option)
: TGraphErrors(filename, format, option),
  fFlashPoint(0),
  fFlashMarker(0)
{
  // AliROMGraphErrors constructor reading input from filename
}
//______________________________________________________________________________
AliROMGraphErrors::~AliROMGraphErrors()
{
  // AliROMGraphErrors destructor
  if (fFlashMarker) delete fFlashMarker;
}
//______________________________________________________________________________
void AliROMGraphErrors::Draw(Option_t *chopt)
{
  TGraphErrors::Draw(chopt);
  if (fFlashMarker) fFlashMarker->Draw();
}
//______________________________________________________________________________
Int_t AliROMGraphErrors::DistancetoPrimitive(Int_t px, Int_t py)
{
  Int_t ret = TGraphErrors::DistancetoPrimitive(px, py);
  if (!fFlashMarker) return ret;
  if (ret == 9999) return ret; // point is not in the graph area

  Int_t pxp, dx, dxmax = 10, prevp = fFlashPoint;
  for (Int_t i = 0; i < fNpoints; i++) {
    pxp = gPad->XtoAbsPixel(gPad->XtoPad(fX[i]));
    dx  = TMath::Abs(pxp-px);
    if (dx < dxmax) {
      dxmax = dx;
      fFlashPoint = i;
    }
  }

  if (fFlashPoint != prevp) FlashPoint(kTRUE);
  return ret;
}
//______________________________________________________________________________
char *AliROMGraphErrors::GetObjectInfo(Int_t px, Int_t py) const
{
  if (!gPad) return (char *)"";

  return TGraphErrors::GetObjectInfo(px, py);
  // ToDo
  //  static char info[64];
  //  Double_t x = gPad->AbsPixeltoX(px);
  //  Double_t y = gPad->AbsPixeltoY(py);
  //  const char *xy = "X";
  //  if (strstr(GetTitle(), "y") || strstr(GetTitle(), "Y")) xy = "Y";
  //  sprintf(info, "Z=%7.2f, %s=%7.2f [cm]", gPad->PadtoX(x), xy, gPad->PadtoY(y));
  //  return info;
}
//______________________________________________________________________________
void AliROMGraphErrors::SetShowHisto(Option_t* option)
{
  if (fFlashMarker) return;

  TVirtualPad *save = gPad;
  Int_t ww = 610;
  TCanvas *c = new TCanvas("c_graph", GetTitle(), gClient->GetDisplayWidth()-ww, 0, ww, ww*1.20);
  c->Connect("Closed()", "AliROMGraphErrors", this, "FlashPoint(=kFALSE)"); // emitted 2x signal ?!
  TString opt = option;
  c->Divide(2, 3, 0.001, 0.001);
  c->GetPad(3)->SetGrid();
  c->GetPad(4)->SetGrid();
  if (save) save->cd();
  fFlashPoint = 0; // first point from graph
  FlashPoint(kTRUE);
}
//______________________________________________________________________________
void AliROMGraphErrors::ShowHisto(Option_t * /*option*/) const
{
  TVirtualPad *save = gPad;
  TCanvas *c = (TCanvas *)gROOT->GetListOfCanvases()->FindObject("c_graph");
  if (!c) return;
  c->Clear("D");
  TDatime today;
  TLatex latex;
  c->cd(1);
  latex.SetTextSize(0.05);
  latex.DrawLatex(0.5, 0.5, today.AsString());
  c->Update();
  if (save) save->cd();
}
//______________________________________________________________________________
void AliROMGraphErrors::FlashPoint(Bool_t flash)
{
  if (!flash) {
    if (fFlashMarker) {
      delete fFlashMarker;
      fFlashMarker = 0;
      fFlashPoint = 0;
    }
  }
  else {
    if (!fFlashMarker) {
      fFlashMarker = new TMarker(fX[fFlashPoint], fY[fFlashPoint], GetMarkerStyle());
      fFlashMarker->SetBit(kCannotPick);
      fFlashMarker->Draw();
    }
    fFlashMarker->SetMarkerStyle(GetMarkerStyle());
    fFlashMarker->SetMarkerSize(GetMarkerSize()*1.66);
    fFlashMarker->SetMarkerColor(GetMarkerColor());
    fFlashMarker->SetX(fX[fFlashPoint]);
    fFlashMarker->SetY(fY[fFlashPoint]);
    ShowHisto();
  }

  if (gPad) {
    gPad->Update();
    gPad->Modified();
  }
}
//______________________________________________________________________________
