#include <TH2.h>
#include <THStack.h>
#include <TPaletteAxis.h>
#include <TString.h>
#include <TObjArray.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TList.h>
#include <TObject.h>
#include <TFile.h>
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
#include <cmath>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using std::cout;
using std::endl;

namespace plot{
const double  kMin=-99999999999999999;
const double kMax=-999999999999999999;
}

TString gTidyPlotCurrentPlotName;
void SavePlot(const char* extension=0,const char* directory=NULL, const TString filename="",bool makeRootFile=true){
  if(gTidyPlotCurrentPlotName.Length()==0) {
    cout<<"Error: It looks like there is no current plot (gTidyPlotCurrentPlotName is empty)"<<endl;
    return;
  }
  TString name=Form("Tidied_%s",gTidyPlotCurrentPlotName.Data());
  if(filename.Length()>0) name=filename;
  if(directory) name=Form("%s/%s",directory,name.Data());

  if(!extension){
    gPad->SaveAs(Form("%s.pdf",name.Data()));
    gPad->SaveAs(Form("%s.png",name.Data()));
  } else{
    gPad->SaveAs(Form("%s.%s",name.Data(),extension));
  }
  if(makeRootFile) gPad->SaveAs(Form("%s.root",name.Data()));

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

void SetHistTitle(TNamed* hist,TString given_title,TString x_axis_label,TString y_axis_label,TString z_axis_label){
	if(!hist) return;
	if(TString(hist->ClassName())!="THStack") return;
	TString curr_full_title=hist->GetTitle();
        TObjArray* all_titles=curr_full_title.Tokenize(";");
	if(all_titles && all_titles->GetEntries()>0){
		if(given_title.Length()==0) given_title=all_titles->At(0)->GetName();
		if(x_axis_label.Length()==0 && all_titles->GetEntries()>1) x_axis_label=all_titles->At(1)->GetName();
		if(y_axis_label.Length()==0 && all_titles->GetEntries()>2) y_axis_label=all_titles->At(2)->GetName();
		if(z_axis_label.Length()==0 && all_titles->GetEntries()>3) z_axis_label=all_titles->At(3)->GetName();
	}
	TString final_title=given_title+";";
	final_title+=x_axis_label+";";
	final_title+=y_axis_label+";";
	final_title+=z_axis_label+";";
	hist->SetTitle(final_title.Data());
}

#define LOOP_STACK(list,body)\
    TList* all_hists=list; \
    TIterator* iterator=all_hists->MakeIterator();\
    TObject* object=NULL;\
    while((object=iterator->Next())){\
	    body\
    }

void RebinStack(THStack* stack,TString axis, int rebin){
     LOOP_STACK(stack->GetHists(),
        if( object->InheritsFrom("TH2") && axis=="y"){
            ((TH2*)object)->RebinY(rebin);
        } else if( object->InheritsFrom("TH1") && axis=="x"){
            ((TH1*)object)->Rebin(rebin);
        }else continue;
     )
}

void NormaliseStack(THStack* stack,double normalise){
    LOOP_STACK(stack->GetHists(),
        if( object->InheritsFrom("TH1")){
            ((TH1*)object)->Scale(1/normalise);
        }else continue;
    )
}

void NormaliseGraph(TGraph* graph,double normalise){
	const int N=graph->GetN();
	double x=0, y=0;
	for(int i=0;i<N;++i){
		graph->GetPoint(i,x,y);
		graph->SetPoint(i,x,y/normalise);
	}
}

void NormaliseMultiGraph(TMultiGraph* mg,double normalise){
    LOOP_STACK(mg->GetListOfGraphs(),
        if( object->InheritsFrom("TGraph")){
		NormaliseGraph((TGraph*)object,normalise);
        }else continue;
    )
}

void StackSetLineWidth(THStack* stack,double line_width){
    LOOP_STACK(stack->GetHists(),
        if( object->InheritsFrom("TH1")){
            ((TH1*)object)->SetLineWidth(line_width);
        }else continue;
    )
}

void StackSetFillTransparency(THStack* stack,double fill_transparency){
    LOOP_STACK(stack->GetHists(),
        if( object->InheritsFrom("TH1")){
            ((TH1*) object)->SetFillColorAlpha(((TH1*) object)->GetFillColor(),fill_transparency);
        }else continue;
    )
}

void MultiGraphSetLineWidth(TMultiGraph* graph, double line_width){
    LOOP_STACK(graph->GetListOfGraphs(),
        if( object->InheritsFrom("TGraph")){
            ((TGraph*)object)->SetLineWidth(line_width);
	    cout<<"Changing line width of "<<graph->GetName()<<" to "<<line_width<<endl;
        }else continue;
    )
}

TLegend* BuildLegend(THStack* stack,TString legend_build_from,TString legend_entry_formula_str ,TString legend_entry_format_str ){
    legend_build_from.ToLower();
    if(legend_build_from!="index" && legend_build_from!="legend_obj" && legend_build_from!="line_colour"){
	    cout<<"Error: Cannot build a legend using any value but the histogram's 'index' or the TNamed 'legend_obj' in the stack (at this time)"<<endl;
	    return NULL;
    }

    TFormula* formula=NULL;
    if (legend_entry_formula_str.Length()>0){
	    formula=new TFormula("legend_formula",legend_entry_formula_str.Data());
    }
   TList* all_hists=stack->GetHists();
   int entries=all_hists->GetEntries();
   typedef std::map<double,std::pair<TH1*,TString> > SortedEntryList;
   SortedEntryList sortedEntries;
   TLegend* legend=new TLegend();
   for(int i=0; i<entries;++i){
	TObject* obj=all_hists->At(i);
	if(!obj->InheritsFrom("TH1")) continue;
	TH1* hist=(TH1*)obj;

	TString entry_name;
	if(legend_build_from=="legend_obj"){
		TNamed* legend_obj=Find<TNamed>(hist->GetListOfFunctions(), "legend");
		if(legend_obj){
			entry_name=legend_obj->GetTitle();
			legend->AddEntry(hist,entry_name,"l");
			continue;
		}
	}
	if(entry_name.Length()==0){
		double legend_value=i;
		if(legend_build_from=="line_colour"){
			legend_value=(hist->GetLineColor()-gStyle->GetColorPalette(0))*1.0/gStyle->GetNumberOfColors();
			cout<<"[BuildLegend]  Entry index="<<i<<", colour= "<<legend_value<<endl;
		}
		if(formula) legend_value=formula->Eval(legend_value);
		entry_name=Form(legend_entry_format_str,legend_value);
		sortedEntries[legend_value]=std::make_pair(hist,entry_name);
	}
   }
   for(SortedEntryList::const_iterator i_entry=sortedEntries.begin();
	i_entry!=sortedEntries.end(); ++i_entry){
	legend->AddEntry(i_entry->second.first,i_entry->second.second.Data(),"l");
   }
   return legend;
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
  void SetLineAppearance(int colour, int style, int weight=1){
    line->SetLineColor(colour);
    line->SetLineStyle(style);
    line->SetLineWidth(weight);
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

TString ParseAxisText(TString input_text,TH1* axes,int rebin_x,int rebin_y){
    typedef std::map<const char*,const char*> ReplacementList;
    ReplacementList replacements;
    std::stringstream sstream;
    sstream<<axes->GetXaxis()->GetBinWidth(1)*rebin_x;
    replacements["{x_bin_widths}"]=sstream.str().c_str();
    sstream.str(""); sstream.flush();
    sstream<<axes->GetYaxis()->GetBinWidth(1)*rebin_y;
    replacements["{y_bin_widths}"]=sstream.str().c_str();

    // remove all anchors from text
    for(ReplacementList::const_iterator i_repl=replacements.begin();
            i_repl!= replacements.end(); ++i_repl){
	    input_text.ReplaceAll(i_repl->first,i_repl->second);
    }
    cout<<input_text<<endl;
    return input_text;
}

struct PlotConfig{
   TString title;
   TString x_axis_label;
   TString y_axis_label;
   TString z_axis_label;

   double x_axis_label_offset;
   double y_axis_label_offset;
   double z_axis_label_offset;
   double x_axis_title_size;
   double y_axis_title_size;
   double x_axis_label_size;
   double y_axis_label_size;

   int x_axis_decimal;
   int y_axis_decimal;
   int x_axis_divisions;
   int y_axis_divisions;

   double x_axis_range_high, x_axis_range_low;
   double y_axis_range_high, y_axis_range_low;
   double z_axis_range_high, z_axis_range_low;

   bool z_axis_label_centred;

   TString legend_header;
   double legend_x1, legend_x2, legend_y1,legend_y2;
   int legend_columns;
   double legend_margin;
   double legend_text_size;
   bool legend_remove;
   TString legend_build_from;
   TString legend_entry_format;
   TString legend_entry_formula;

   double shift_plot_x;
   double shift_plot_y;

   double shift_palette;

   double pad_margin_right;
   double pad_margin_left;

   int marker_color;
   double marker_size;
   double line_width;
   double fill_transparency;

   int rebin_x;
   int rebin_y;
   double normalise;

   int canvas_width;
   int canvas_height;
   double canvas_grow;
   double canvas_ratio;

   TString force_draw_option;

   bool stats_force_off;
   bool log_z;
   bool log_y;
   bool log_x;
   bool grid_y;
   bool grid_x;


  private:
   std::vector<AnnotatedLine*> lines;
   TLegend* _legend;
   TPaletteAxis* _palette;

  public:

   //TPoint frame_centre;

   PlotConfig(){reset();}
   void reset(){
     x_axis_label_offset=0;
     y_axis_label_offset=0;
     z_axis_label_offset=0;
     x_axis_title_size=0;
     y_axis_title_size=0;
     x_axis_label_size=0;
     y_axis_label_size=0;
     x_axis_range_high=x_axis_range_low=0;
     y_axis_range_high=y_axis_range_low=0;
     z_axis_range_high=z_axis_range_low=0;
     legend_x1=legend_x2=legend_y1=legend_y2=-1;
     legend_margin=0;
     legend_columns=0;
     legend_text_size=0;
     legend_remove=false;
     legend_build_from="";
     legend_entry_format="%lg";
     legend_entry_formula="";
     x_axis_decimal=-1;
     y_axis_decimal=-1;
     x_axis_divisions=-1;
     y_axis_divisions=-1;
     shift_plot_x=0;
     shift_plot_y=0;
     shift_palette=0;
     pad_margin_right=0;
     pad_margin_left=0;
     title="";
     legend_header="";
     x_axis_label="";
     y_axis_label="";
     z_axis_label="";
     z_axis_label_centred=false;
     stats_force_off=false;
     force_draw_option="";
     rebin_x=1;
     rebin_y=1;
     normalise=1;
     canvas_width=0;
     canvas_height=0;
     canvas_grow=1;
     canvas_ratio=1;
     marker_color=kBlack;
     marker_size=0;
     line_width=1;
     fill_transparency=1;
     ClearLines();
     _legend=NULL;
     _palette=NULL;
     log_z=log_y=log_x=false;
     grid_x=grid_y=false;
   }
   void AddLine(AnnotatedLine& line){
       lines.push_back(&line);
   }
   void ClearLines(){lines.clear();}
   void ApplyFixes(TH1* axes, TLegend* legend,TNamed* hist);
   void FixCanvas();
   void UpdateEverything(TNamed*);
   void RedrawLegend();
};

void PlotConfig::FixCanvas(){
  if(stats_force_off){
    gStyle->SetOptStat(0);
  }
  if(pad_margin_left!=0){
      gPad->ResetAttPad();
      gPad->SetLeftMargin(pad_margin_left);
  }
  if(pad_margin_right!=0){
      gPad->SetRightMargin(pad_margin_right);
  }
  if(shift_plot_x!=0){
    gPad->SetLeftMargin( gPad->GetLeftMargin()+shift_plot_x);
    gPad->SetRightMargin( gPad->GetRightMargin()-shift_plot_x);
    gStyle->SetBarOffset(0);
  }
  if(shift_plot_y!=0){
    gPad->SetTopMargin( gPad->GetTopMargin()+shift_plot_y);
    gPad->SetBottomMargin( gPad->GetBottomMargin()-shift_plot_y);
  }
  if(canvas_height!=0){
	  int width=gPad->GetWw();
	  gPad->SetCanvasSize(width,canvas_height);
  }
  if(canvas_width!=0){
	  int height=gPad->GetWh();
	  gPad->SetCanvasSize(canvas_width,height);
  }
  if(canvas_grow!=1){
	  int width=gPad->GetWw();
	  int height=gPad->GetWh();
	  gPad->SetCanvasSize(width*canvas_grow,height*canvas_grow);
  }
  if(canvas_ratio!=1){
	  int width=gPad->GetWw();
	  int height=gPad->GetWh();
	  int area=width*height;
	  double mean=sqrt(area);
	  gPad->SetCanvasSize(mean*canvas_ratio,mean/canvas_ratio);
  }
  if(log_x)gPad->SetLogx(); else gPad->SetLogx(0);
  if(log_y)gPad->SetLogy(); else gPad->SetLogy(0);
  if(log_z)gPad->SetLogz(); else gPad->SetLogz(0);
  if(grid_x)gPad->SetGridx(); else gPad->SetGridx(0);
  if(grid_y)gPad->SetGridy(); else gPad->SetGridy(0);

  gPad->Update();
  gPad->Draw();
}

void PlotConfig::ApplyFixes( TH1* axes, TLegend* legend,TNamed* hist){
  if(axes){
    TString curr_title=axes->GetTitle();
    if(curr_title.Contains(";")){
        TObjArray* all_titles=curr_title.Tokenize(";");
        if(title.Length()==0) title=all_titles->At(0)->GetName();
        if(x_axis_label.Length()==0 && all_titles->GetEntries()>1) x_axis_label=all_titles->At(1)->GetName();
        if(y_axis_label.Length()==0 && all_titles->GetEntries()>2) y_axis_label=all_titles->At(2)->GetName();
        if(z_axis_label.Length()==0 && all_titles->GetEntries()>3) z_axis_label=all_titles->At(3)->GetName();
    }
    if(title.Length()!=0) {
        axes->SetTitle(title.Data());
        if(hist)hist->SetTitle(title.Data());
    }
    if(x_axis_range_high != x_axis_range_low){
      axes->GetXaxis()->UnZoom();
      axes->GetXaxis()->SetRangeUser(x_axis_range_low,x_axis_range_high);
    }
    if(y_axis_range_high != y_axis_range_low){
	    axes->GetYaxis()->UnZoom();
      	    axes->GetYaxis()->SetRangeUser(y_axis_range_low,y_axis_range_high);
	    cout<<"Changed y axis: "<<y_axis_range_low<<" to "<<y_axis_range_high<<endl;
    }
    if(z_axis_range_high != z_axis_range_low){
      axes->GetZaxis()->UnZoom();
      axes->GetZaxis()->SetRangeUser(z_axis_range_low,z_axis_range_high);
    }
    if(x_axis_label_offset!=0)   axes->GetXaxis()->SetTitleOffset(x_axis_label_offset);
    if(y_axis_label_offset!=0)   axes->GetYaxis()->SetTitleOffset(y_axis_label_offset);
    if(z_axis_label_offset!=0)   axes->GetZaxis()->SetTitleOffset(z_axis_label_offset);
    if(x_axis_label_size!=0)   axes->GetXaxis()->SetLabelSize(x_axis_label_size);
    if(y_axis_label_size!=0)   axes->GetYaxis()->SetLabelSize(y_axis_label_size);
    if(x_axis_title_size!=0)   axes->GetXaxis()->SetTitleSize(x_axis_title_size);
    if(y_axis_title_size!=0)   axes->GetYaxis()->SetTitleSize(y_axis_title_size);
    if(x_axis_decimal!=-1)   axes->GetXaxis()->SetDecimals(x_axis_decimal);
    if(y_axis_decimal!=-1)   axes->GetYaxis()->SetDecimals(y_axis_decimal);
    if(x_axis_label.Length()!=0) axes->GetXaxis()->SetTitle(ParseAxisText(x_axis_label,axes,rebin_x,rebin_y).Data());
    if(y_axis_label.Length()!=0) axes->GetYaxis()->SetTitle(ParseAxisText(y_axis_label,axes,rebin_x,rebin_y).Data());
    if(z_axis_label.Length()!=0) axes->GetZaxis()->SetTitle(ParseAxisText(z_axis_label,axes,rebin_x,rebin_y).Data());
    // Need to reset the original histogram title in some cases (eg. THStacks)
    SetHistTitle(hist,title,x_axis_label,y_axis_label,z_axis_label);
    if(axes->GetZaxis())axes->GetZaxis()->CenterTitle(z_axis_label_centred);
  }

  if(hist){
    if(force_draw_option!="") hist->Draw(force_draw_option.Data());
    if(rebin_x!=1 ){
	    if(hist->InheritsFrom("TH1")){
	        ((TH1*) hist)->RebinX(rebin_x);
	    }
	    else if(hist->InheritsFrom("THStack")){
		    RebinStack((THStack*) hist,"x",rebin_x);
	    }else{
		    cout<<"Error: Cannot rebin histogram that is not derived from TH1 or THStack"<<endl;
	    }
    }
    if(rebin_y!=1 ){
	    if(hist->InheritsFrom("TH2")){
	        ((TH2*) hist)->RebinY(rebin_y);
	    } else if(hist->InheritsFrom("THStack")){
		    RebinStack((THStack*) hist,"y",rebin_y);
	    }else{
		    cout<<"Error: Cannot rebin 2D histogram in Y if it is not derived from TH2 or THStack"<<endl;
	    }
    }
    if(x_axis_divisions!=-1){
        if(hist->InheritsFrom("TH1")){
            ((TH1*) hist)->SetNdivisions(x_axis_divisions,"x");
	}else if(hist->InheritsFrom("TH1")){
            ((THStack*) hist)->GetHistogram()->SetNdivisions(x_axis_divisions,"x");
        }
    }
    if(y_axis_divisions!=-1){
        if(hist->InheritsFrom("TH1")){
            ((TH1*) hist)->SetNdivisions(y_axis_divisions,"y");
	}else if(hist->InheritsFrom("TH1")){
            ((THStack*) hist)->GetHistogram()->SetNdivisions(y_axis_divisions,"y");
        }
    }
    if(normalise!=1){
      if(hist->InheritsFrom("TH1")){
          ((TH1*) hist)->Scale(1/normalise);
      }
      else if(hist->InheritsFrom("THStack")){
              NormaliseStack((THStack*) hist,normalise);
      }else if(hist->InheritsFrom("TGraph")){
              NormaliseGraph((TGraph*) hist,normalise);
      }else if(hist->InheritsFrom("TMultiGraph")){
              NormaliseMultiGraph((TMultiGraph*) hist,normalise);
      }else{
              cout<<"Error: Cannot normalise plot that is not derived from TH1, THStack, TMultiGraph"<<endl;
      }
    }
    if(marker_size!=0){
      if(hist->InheritsFrom("TH1")){
          ((TH1*) hist)->SetMarkerSize(marker_size);
      }
    }
    if(marker_color!=kBlack){
      if(hist->InheritsFrom("TH1")){
          ((TH1*) hist)->SetMarkerColor(marker_color);
      //} else if(hist->InheritsFrom("THStack")){
      //        StackSetLineWidth((THStack*) hist,line_width);
      //}else if(hist->InheritsFrom("TMultiGraph")){
      //        MultiGraphSetLineWidth((TMultiGraph*) hist,line_width);
      }else{
              cout<<"Error: Cannot change marker colour that is not derived from TH1 "<<endl;
      }
    }
    if(fill_transparency!=1){
      if(hist->InheritsFrom("TH1")){
          ((TH1*) hist)->SetFillColorAlpha(((TH1*) hist)->GetFillColor(),fill_transparency);
      } else if(hist->InheritsFrom("THStack")){
              StackSetFillTransparency((THStack*) hist,fill_transparency);
      }else if(hist->InheritsFrom("TGraph")){
          ((TGraph*) hist)->SetFillColorAlpha(((TGraph*) hist)->GetFillColor(),fill_transparency);
      //}else if(hist->InheritsFrom("TMultiGraph")){
      //        MultiGraphSetLineWidth((TMultiGraph*) hist,fill_transparency);
      }else{
              cout<<"Error: Cannot set transparency for objects not derived from TH1, THStack, TGraph"<<endl;
      }
    }
    if(line_width!=1){
      if(hist->InheritsFrom("TH1")){
          ((TH1*) hist)->SetLineWidth(line_width);
      } else if(hist->InheritsFrom("THStack")){
              StackSetLineWidth((THStack*) hist,line_width);
      }else if(hist->InheritsFrom("TGraph")){
          ((TGraph*) hist)->SetLineWidth(line_width);
      }else if(hist->InheritsFrom("TMultiGraph")){
              MultiGraphSetLineWidth((TMultiGraph*) hist,line_width);
      }else{
              cout<<"Error: Cannot rebin histogram that is not derived from TH1 or THStack"<<endl;
      }
    }
    if(y_axis_range_high != y_axis_range_low){
      if(hist->InheritsFrom("TH1")){
          ((TH1*) hist)->GetYaxis()->SetRangeUser(y_axis_range_low,y_axis_range_high);
      } else if(hist->InheritsFrom("THStack")){
          ((THStack*) hist)->SetMaximum(y_axis_range_high);
          ((THStack*) hist)->SetMinimum(y_axis_range_low);
      }else if(hist->InheritsFrom("TMultiGraph")){
          ((TMultiGraph*) hist)->GetYaxis()->SetRangeUser(y_axis_range_low,y_axis_range_high);
      }else{
              cout<<"Error: Cannot rebin histogram that is not derived from TH1 or THStack"<<endl;
      }
    }
    if(x_axis_range_high != x_axis_range_low){
      if(hist->InheritsFrom("TH1")){
          ((TH1*) hist)->GetXaxis()->SetRangeUser(x_axis_range_low,x_axis_range_high);
      //} else if(hist->InheritsFrom("THStack")){
      }else if(hist->InheritsFrom("TMultiGraph")){
          ((TMultiGraph*) hist)->GetXaxis()->SetRangeUser(x_axis_range_low,x_axis_range_high);
      }else{
              cout<<"Error: Cannot rebin histogram that is not derived from TH1 or THStack"<<endl;
      }
    }
    if(hist->InheritsFrom("TH1")){
	_palette=(TPaletteAxis*)((TH1*)hist)->GetListOfFunctions()->FindObject("palette");
    }
    if(_palette && shift_palette!=0){
       _palette->SetX1NDC(_palette->GetX1NDC()+shift_palette);
       _palette->SetX2NDC(_palette->GetX2NDC()+shift_palette);
    }
  }

  if(legend_build_from.Length()!=0){
      if(hist->InheritsFrom("THStack")){
	      // hide the existing legend if it exists
	      if(legend){
		  legend->SetX1NDC(1.1);
		  legend->SetX2NDC(1.5);
	      }
	      legend=BuildLegend((THStack*)hist,legend_build_from,legend_entry_formula,legend_entry_format);
      }else{
              cout<<"Error: Cannot add a legend if plot is not made from a THStack"<<endl;
      }
  }
  if(legend){
	  _legend=legend;
    if(legend_x1!=-1) legend->SetX1NDC(legend_x1);
    if(legend_x2!=-1) legend->SetX2NDC(legend_x2);
    if(legend_y1!=-1) legend->SetY1NDC(legend_y1);
    if(legend_y2!=-1) legend->SetY2NDC(legend_y2);
    if(legend_margin!=0) legend->SetMargin(legend_margin);
    if(legend_columns!=0) legend->SetNColumns(legend_columns);
    if(legend_text_size!=0) legend->SetTextSize(legend_text_size);
    if(legend_header.Length()!=0) legend->SetHeader(legend_header.Data());
    if(legend_remove) {
	    legend->SetX1NDC(1.1);
	    legend->SetX2NDC(1.5);
    }
  }
  if(shift_plot_x!=0){
    if(legend) legend->SetX1NDC(legend->GetX1NDC()+shift_plot_x);
    if(legend) legend->SetX2NDC(legend->GetX2NDC()+shift_plot_x);
    if(_palette) _palette->SetX1NDC(_palette->GetX1NDC()+shift_plot_x);
    if(_palette) _palette->SetX2NDC(_palette->GetX2NDC()+shift_plot_x);
  }
  if(shift_plot_y!=0){
    if(legend) legend->SetY1NDC(legend->GetY1NDC()+shift_plot_y);
    if(legend) legend->SetY2NDC(legend->GetY2NDC()+shift_plot_y);
    if(_palette) _palette->SetY1NDC(_palette->GetY1NDC()+shift_plot_y);
    if(_palette) _palette->SetY2NDC(_palette->GetY2NDC()+shift_plot_y);
  }
}

void PlotConfig::UpdateEverything(TNamed* hist){
  gPad->Update();
  for(std::vector<AnnotatedLine*>::const_iterator i_line=lines.begin();
      i_line!=lines.end(); ++i_line){
    (*i_line)->Draw();
  }
  if(_legend) _legend->Draw();
}

void PlotConfig::RedrawLegend(){
  if(_legend) _legend->Draw();
}

void FixCanvas(TCanvas* canvas,
    const TString plot_type,
    PlotConfig& config){
  // Get the legend object (if any)
  TLegend* legend=Find<TLegend>(canvas->GetListOfPrimitives());
  if(!legend){
    cout<<"Warning: No legend for plot in file: '"<<gTidyPlotCurrentPlotName<<"'"<<endl;
  }

  // Change the canvas as wanted
  config.FixCanvas();

  // Get the axis for the histogram
  TH1* axis=NULL;
  TNamed* hist=NULL;
  if(plot_type=="THStack"){
    THStack* stack=Find<THStack>(canvas->GetListOfPrimitives());
    if(stack) {
      axis=stack->GetHistogram();
      hist=stack;
    }
  }else if(plot_type=="TMultiGraph"){
    TMultiGraph* plot=Find<TMultiGraph>(canvas->GetListOfPrimitives());
    if(plot) {
      axis=plot->GetHistogram();
      hist=plot;
    }
  }else if(plot_type=="TGraph"){
    TGraph* plot=Find<TGraph>(canvas->GetListOfPrimitives());
    if(plot) {
      axis=plot->GetHistogram();
      hist=plot;
    }
  } else{
    axis=Find<TH1>(canvas->GetListOfPrimitives());
    if(!axis) {
       TGraph* graph=Find<TGraph>(canvas->GetListOfPrimitives());
       axis=graph->GetHistogram();
    }
    hist=axis;
  }
  if(!axis){
    cout<<"Warning: Cannot find histogram or stack to draw in file: '"<<gTidyPlotCurrentPlotName<<"'"<<endl;
  }

  // Apply the actual fixes
  config.ApplyFixes(axis,legend,hist);
  config.UpdateEverything(hist);

}

TFile* FixPlot(TString filename,
    const TString plot_type,
    PlotConfig& config){
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

  FixCanvas(canvas,plot_type,config);
  return file;
}

TFile* DrawFixPlot(TString filename,TString histname,
    const TString plot_type,
    PlotConfig& config, const char* draw_string="",bool normToEventsRead=true){
  // Get the file
  TFile* file=TFile::Open(filename.Data(),"READ");
  if(!file || file->IsZombie()){
    std::cout<<"Error: Cannot find file: '"<<filename<<"'"<<std::endl;
    return NULL;
  }

  // Obtain the object to draw
  TObject* object=file->Get(histname.Data());
  if(!object) {
    std::cout<<"Error: Cannot find object in file: '"<<filename<<"' called: '"<<histname<<"'"<<std::endl;
    return NULL;
  }
  object->Draw(draw_string);

  if(normToEventsRead){
    // Obtain the object's normalisation
    TNamed* norm_obj=(TNamed*)file->Get("events_read");
    if(norm_obj){
	    std::stringstream sstream(norm_obj->GetTitle());
	    double norm=0;
	    sstream>>norm;
	    config.normalise=norm;
    }
  }

  // Set the current file name
  gTidyPlotCurrentPlotName=filename;
  gTidyPlotCurrentPlotName.Remove(gTidyPlotCurrentPlotName.Last('.'));
  gTidyPlotCurrentPlotName.Remove(0,gTidyPlotCurrentPlotName.Last('/')+1);
  gTidyPlotCurrentPlotName+=histname.ReplaceAll("/","--");

  FixCanvas(gPad->GetCanvas(),plot_type,config);
  return file;
}

void Overlay(TNamed* object,const char* options, PlotConfig& config){
	object->Draw(options);
	config.RedrawLegend();
}
