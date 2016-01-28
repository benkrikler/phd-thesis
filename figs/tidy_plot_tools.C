#include <iostream>
#include <TString.h>
#include <TCanvas.h>
#include <TList.h>
#include <TObject.h>
#include <TFile.h>
#include <THStack.h>
#include <TH1.h>
#include <TLegend.h>
#include <TKey.h>
#include <TPoint.h>
#include <TFrame.h>
#include <TMultiGraph.h>
#include <TLine.h>
#include <TLatex.h>
#include <TVector2.h>
#include <TMath.h>
#include <TStyle.h>
#include <vector>

using std::cout;
using std::endl;

namespace plot{
const double  kMin=-99999999999999999;
const double kMax=-999999999999999999;
}

TString gTidyPlotCurrentPlotName;
void SavePlot(const char* filename=0,const char* extension=0,bool makeRootFile=true){
  if(gTidyPlotCurrentPlotName.Length()==0) {
    cout<<"Error: It looks like there is no current plot (gTidyPlotCurrentPlotName is empty)"<<endl;
    return;
  }
  const char* name=Form("Tidied_%s",gTidyPlotCurrentPlotName.Data());
  if(filename) name=filename;

  if(!extension){
    gPad->Print(Form("%s.pdf",name));
    gPad->Print(Form("%s.png",name));
  } else{
    gPad->Print(Form("%s.%s",name,extension));
  }
  if(makeRootFile) gPad->Print(Form("%s.root",name));

}

template <typename TYPE>
TYPE* Find(TList* list, const TString& name=""){
    TIter next_obj(list);
    while(TObject* obj=next_obj()){
      if(obj->IsA()==TKey::Class()){
        obj=((TKey*)obj)->ReadObj();
      }
      TYPE* concrete=dynamic_cast<TYPE*>(obj);
      if(concrete && ( name.Length()==0 || concrete->GetName()==name )){
        return concrete;
      }
    }
    return NULL;
}

struct AnnotatedLine{
  enum HorizontalTextAlign_t{ kBegin, kCentre, kEnd} text_pos_x;
  enum VerticalTextAlign_t{ kAbove, kOver, kBelow} text_pos_y;
  double text_pos_x_displacement;
  double text_pos_y_displacement;
  TLine* line;
  TLatex* text;

  AnnotatedLine(){
    line=new TLine();
    text=new TLatex();
  } 
  AnnotatedLine(const char* label, double x1, double y1, double x2, double y2):
    text_pos_x(kCentre),text_pos_y(kAbove),text_pos_x_displacement(0),text_pos_y_displacement(0)
  {
    line=new TLine(x1,y1,x2,y2);
    text=new TLatex(0,0,label);
    text->SetTextSize(0.8*gStyle->GetLabelSize());
    text->SetTextFont(gStyle->GetLabelFont());
  }
  void SetTextAlignHor(HorizontalTextAlign_t align, double displace=0){
    text_pos_x=align; text_pos_x_displacement=displace;
  }
  void SetTextAlignVer(VerticalTextAlign_t align, double displace=0){
    text_pos_y=align; text_pos_y_displacement=displace;
  }
  void SetLineAppearance(int colour, int style){
    line->SetLineColor(colour);
    line->SetLineStyle(style);
  }
  void Draw()const{
    double x1=line->GetX1(),x2=line->GetX2(),y1=line->GetY1(), y2=line->GetY2();
    cout<<x1<<", "<<y1<<", "<<x2<<", "<<y2<<", "<<plot::kMax<<endl;
    double* vals_x[2]={&x1,&x2};
    for(int i=0; i<2; ++i){
        cout<<" value ("<<i<<") = "<<*vals_x[i]<<endl;
             if(*vals_x[i]==plot::kMax) *vals_x[i]=gPad->GetUxmax();
        else if(*vals_x[i]==plot::kMin) *vals_x[i]=gPad->GetUxmin();
        cout<<" value ("<<i<<") = "<<*vals_x[i]<<endl;
    }
    double* vals_y[2]={&y1,&y2};
    for(int i=0; i<2; ++i){
        cout<<" value ("<<i<<") = "<<*vals_y[i]<<endl;
             if(*vals_y[i]==plot::kMax) *vals_y[i]=gPad->GetUymax();
        else if(*vals_y[i]==plot::kMin) *vals_y[i]=gPad->GetUymin();
        cout<<" value ("<<i<<") = "<<*vals_y[i]<<endl;
    }

    TVector2 dir(x2-x1, y2-y1);
    const double angle=dir.Phi();
    TVector2 orth=dir.Rotate(TMath::PiOver2()).Unit();
    double x_pos=0;
    int v_align=text->GetTextAlign()%10;
    int h_align=2;
    switch(text_pos_x){
      case kEnd:    x_pos=1;   h_align=3; break;
      case kBegin:  x_pos=0;   h_align=1; break;
      case kCentre: x_pos=0.5; h_align=2; break;
    }
    dir*=x_pos+text_pos_x_displacement;

    switch(text_pos_y){
      case kAbove: dir+=(10*text->GetTextSize()+text_pos_y_displacement)*orth; v_align=1; break;
      case kBelow: dir+=(10*text->GetTextSize()+text_pos_y_displacement)*orth; v_align=3; break;
      case kOver:  dir+=text_pos_y_displacement*orth;                          v_align=2; break;
    }
    text->SetX(x1+dir.X());
    text->SetY(y1+dir.Y());
    text->SetTextAlign(h_align*10+v_align);
    text->SetTextAngle(angle/TMath::Pi()*180);

    text->Draw();
    line->DrawLine(x1,y1,x2,y2);
  }
};

struct PlotConfig{
   TString x_axis_label;
   TString y_axis_label;

   double x_axis_label_offset;
   double y_axis_label_offset;

   int x_axis_decimal;
   int y_axis_decimal;

   double x_axis_range_high, x_axis_range_low;
   double y_axis_range_high, y_axis_range_low;

   TString legend_header;
   double legend_x1, legend_x2, legend_y1,legend_y2;
   int legend_columns;
   double legend_margin;
   double legend_text_size;

   double shift_plot_x;
   double shift_plot_y;

   bool stats_force_off;

  private:
   std::vector<AnnotatedLine*> lines;

  public:

   //TPoint frame_centre;

   PlotConfig(){reset();}
   void reset(){
     x_axis_label_offset=0;
     y_axis_label_offset=0;
     x_axis_range_high=x_axis_range_low=0;
     y_axis_range_high=y_axis_range_low=0;
     legend_x1=legend_x2=legend_y1=legend_y2=-1;
     legend_margin=0;
     legend_columns=0;
     legend_text_size=0;
     x_axis_decimal=-1;
     y_axis_decimal=-1;
     shift_plot_x=0;
     shift_plot_y=0;
     legend_header="";
     x_axis_label="";
     y_axis_label="";
     stats_force_off=false;
     ClearLines();
   }
   void AddLine(AnnotatedLine& line){
       lines.push_back(&line);
   }
   void ClearLines(){lines.clear();}
   void ApplyFixes(TH1* axes, TLegend* legend)const;
};

void PlotConfig::ApplyFixes( TH1* axes, TLegend* legend)const{
  if(stats_force_off){
    gStyle->SetOptStat(0);
  }
  if(axes){
    if(x_axis_range_high != x_axis_range_low)
      axes->GetXaxis()->SetRangeUser(x_axis_range_low,x_axis_range_high);
    if(y_axis_range_high != y_axis_range_low)
      axes->GetYaxis()->SetRangeUser(y_axis_range_low,y_axis_range_high);
    if(x_axis_label.Length()!=0) axes->GetXaxis()->SetTitle(x_axis_label.Data());
    if(y_axis_label.Length()!=0) axes->GetYaxis()->SetTitle(y_axis_label.Data());
    if(x_axis_label_offset!=0)   axes->GetXaxis()->SetTitleOffset(x_axis_label_offset);
    if(y_axis_label_offset!=0)   axes->GetYaxis()->SetTitleOffset(y_axis_label_offset);
    if(x_axis_decimal!=-1)   axes->GetXaxis()->SetDecimals(x_axis_decimal);
    if(y_axis_decimal!=-1)   axes->GetYaxis()->SetDecimals(y_axis_decimal);
  }
  if(legend){
    if(legend_x1!=-1) legend->SetX1NDC(legend_x1);
    if(legend_x2!=-1) legend->SetX2NDC(legend_x2);
    if(legend_y1!=-1) legend->SetY1NDC(legend_y1);
    if(legend_y2!=-1) legend->SetY2NDC(legend_y2);
    if(legend_margin!=0) legend->SetMargin(legend_margin);
    if(legend_columns!=0) legend->SetNColumns(legend_columns);
    if(legend_text_size!=0) legend->SetTextSize(legend_text_size);
    if(legend_header.Length()!=0) legend->SetHeader(legend_header.Data());
  }
  if(shift_plot_x!=0){
    gPad->SetLeftMargin( gPad->GetLeftMargin()+shift_plot_x);
    gPad->SetRightMargin( gPad->GetRightMargin()-shift_plot_x);
    legend->SetX1NDC(legend->GetX1NDC()+shift_plot_x);
    legend->SetX2NDC(legend->GetX2NDC()+shift_plot_x);
  }
  if(shift_plot_y!=0){
    gPad->SetTopMargin( gPad->GetTopMargin()+shift_plot_y);
    gPad->SetBottomMargin( gPad->GetBottomMargin()-shift_plot_y);
    legend->SetY1NDC(legend->GetY1NDC()+shift_plot_y);
    legend->SetY2NDC(legend->GetY2NDC()+shift_plot_y);
  }
  gPad->Draw();
  for(std::vector<AnnotatedLine*>::const_iterator i_line=lines.begin();
      i_line!=lines.end(); ++i_line){
    (*i_line)->Draw();
  }
  if(legend) legend->Draw();
}

TFile* FixPlot(TString filename,
    const TString plot_type,
    const PlotConfig& config){
  // Get the file
  TFile* file=TFile::Open(filename.Data(),"READ");
  if(!file || file->IsZombie()){
    std::cout<<"Error: Cannot find file: '"<<filename<<"'"<<std::endl;
    return NULL;
  }

  // Obtain the canvas
  TCanvas* canvas=Find<TCanvas>(file->GetListOfKeys());
  if(!canvas) {
    std::cout<<"Error: Cannot find TCanvas object in file: '"<<filename<<"'"<<std::endl;
    return NULL;
  }
  canvas->Draw();

  // Set the current file name
  gTidyPlotCurrentPlotName=filename;
  gTidyPlotCurrentPlotName.Remove(gTidyPlotCurrentPlotName.Last('.'));
  gTidyPlotCurrentPlotName.Remove(0,gTidyPlotCurrentPlotName.Last('/')+1);

  // Get the legend object (if any)
  TLegend* legend=Find<TLegend>(canvas->GetListOfPrimitives());
  if(!legend){
    cout<<"Warning: No legend for plot in file: '"<<filename<<"'"<<endl;
  }

  // Get the axis for the histogram
  TH1* axis=NULL;
  if(plot_type=="THStack"){
    THStack* stack=Find<THStack>(canvas->GetListOfPrimitives());
    if(stack) {
      axis=stack->GetHistogram();
    }
  }else if(plot_type=="TMultiGraph"){
    TMultiGraph* plot=Find<TMultiGraph>(canvas->GetListOfPrimitives());
    if(plot) {
      axis=plot->GetHistogram();
    }
  } else{
    axis=Find<TH1>(canvas->GetListOfPrimitives());
  }
  if(!axis){
    cout<<"Warning: Cannot find histogram or stack to draw in file: '"<<filename<<"'"<<endl;
  }

  // Apply the actual fixes
  config.ApplyFixes(axis,legend);

  return file;
}
