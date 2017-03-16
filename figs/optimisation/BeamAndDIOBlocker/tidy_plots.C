#include "../../tidy_plot_tools.C"

void tidy_plots(const char* ext="png"){
//     gStyle->SetPalette(56);

  //gROOT->ProcessLine(".L ~/thesis/figs/tidy_plot_tools.C+");

  TString top_dir="~/comet/160219_BeamBlocker/plots/";
  PlotConfig config;

  TFile* geomFile=TFile::Open("../Geometry-on-axis.root","READ");
  TH2* geomHist=NULL;
  if(geomFile){
      geomHist=(TH2*)geomFile->Get("hGeometry");
      geomHist->RebinX(4);
      geomHist->RebinY(4);
      for(int ix=0;ix<geomHist->GetNbinsX()+1;ix++){
          for(int iy=0;iy<geomHist->GetNbinsY()+1;iy++){
              double content=geomHist->GetBinContent(ix,iy);
              geomHist->SetBinContent(ix,iy,content>10?10:0);
          }
      }
  }

//  gStyle->SetNumberContours(254);
  gStyle->SetStripDecimals(false);
  gStyle->SetPaintTextFormat(".2f");
  gStyle->SetPalette(54);

  AnnotatedLine StopTgtSec_start("", 16700, plot::kMin, 16700, plot::kMax); StopTgtSec_start.SetLineAppearance(kMagenta, 1 ,2);
  AnnotatedLine        EST_start("", 21800, plot::kMin, 21800, plot::kMax); EST_start.SetLineAppearance       (kMagenta, 2 ,2);
  AnnotatedLine     DetSol_start("", 28000, plot::kMin, 28000, plot::kMax); DetSol_start.SetLineAppearance    (kMagenta, 2 ,2);

  ///////////////////////   Momentum Acceptance 2D
  config.reset();
  //config.title    = ";Radius of Beam Blocker (mm);Height of Top Edge of DIO blockers (mm); Acceptance into Straw Tracker";
  config.x_axis_label= "Radius of Beam Blocker (mm)";
  config.y_axis_label= "Height of Top Edge of DIO blockers (mm)";
  config.z_axis_label= "Acceptance into Straw Tracker";
  config.y_axis_label_size = config.x_axis_label_size=0.03;
  config.y_axis_title_size = config.x_axis_title_size=0.04;
  config.shift_plot_x  = -0.03;
  config.shift_palette_y = -0.05;
  config.canvas_ratio  = 0.80;
  config.canvas_grow  = 2;
  config.canvas_width  = 1500;
  config.stats_force_off  = true;
  config.pad_margin_right = 0.15;
  config.pad_margin_left  = 0.15;
  config.y_axis_label_offset=1.5;
  config.z_axis_label_offset=1.6;
  config.z_axis_label_centred=true;
  config.z_axis_range_high=3;
  config.z_axis_range_low=1e-2;
  config.log_z=true;
  config.force_draw_option="textcolz";
  config.marker_color=kGray;
  config.marker_size=1.6;

  TGaxis::SetMaxDigits(3);
  gStyle->SetPalette(56);
  gStyle->SetHistMinimumZero();

//  top_dir="~/comet/160219_BeamBlocker/plots/";
//  //for(int mom=20; mom<110; mom+=10){
//  for(int mom=40; mom<110; mom+=20){
//     FixPlot(top_dir+Form("plot_Acceptance2D_%d.root",mom),"TH1", config);
//  gStyle->SetPalette(56);
//     SavePlot(ext,NULL,Form("Tidied_Acceptance2D_%d",mom));
//  }

  ///////////////////////   Dispersion of electrons through EST
  config.reset();
  config.x_axis_title_offset= 0.9; config.y_axis_title_offset= 0.6;
  config.x_axis_label_size = 0.04; config.y_axis_label_size = 0.04;
  config.x_axis_title_size = 0.05; config.y_axis_title_size = 0.05;
  config.x_axis_label    = "Distance Along Beamline (mm)";
  config.y_axis_label    = "  Height Above Beamline Axis (mm)";
  config.title = " ";
  //config.z_axis_label_centred= true;
  config.y_axis_range_low = -800;   config.y_axis_range_high = 800; 
  config.x_axis_range_low = 17000; config.x_axis_range_high  = 33000;
  //config.z_axis_range_high  = 5e-4;
  //config.z_axis_range_low  = 5e-6;
  config.canvas_ratio=1.6;
  config.stats_force_off  = true;
  config.line_width=2;
  config.legend_header="Momentum at Target (MeV/c)";
  config.legend_margin=0.6;
  config.legend_text_size=0.035;
  config.grid_x=true; config.grid_y=true;
  config.canvas_grow=2;

  config.legend_build_from="line_colour";
  config.legend_entry_formula="(x-0.0431373)/0.909804*105+5";
  config.legend_entry_format="%2.lf";

  config.AddLine(EST_start);
  config.AddLine(DetSol_start);

  config.legend_x1=0.30; config.legend_y1=0.60;
  config.legend_x2=0.79; config.legend_y2=0.77;
  config.legend_columns=7;
  top_dir="~/comet/160213_StopTgtPositionOptimisation/160816_plots/";
  file=FixPlot(top_dir+"-Heights-Shift_050.root","THStack", config);
  if(geomHist) Overlay(geomHist,"samecol",config);
  SavePlot(ext,NULL,"Tidied_ElectronDispersion-EST");

  return;

  config.fill_transparency=0.3;
  config.legend_x1=0.39; config.legend_y1=0.68;
  config.legend_x2=0.73; config.legend_y2=0.79;
  config.legend_columns=4;
  config.legend_build_from="";
//  file=FixPlot(top_dir+"BeamEnvelope-transparent.root","THStack", config);
//  if(geomHist) Overlay(geomHist,"same",config);
//  SavePlot(ext,NULL,"Tidied_ElectronDispersion-EST-envelope");


  ///////////////////////   Momentum spectra
  config.reset();
  config.line_width =2;
  config.y_axis_label_offset = 1.4;
  config.y_axis_label    = "Rate of Observation (per muon DIO per 4.5 MeV/c)";
  config.x_axis_label    = "Momentum (MeV/c)";
  config.y_axis_decimal  = true;
  config.x_axis_decimal  = true;
  //config.x_axis_range_high  = 120;
  config.legend_text_size= 0.025;
  config.legend_margin= 0.4;
  config.legend_x1  = 0.11 ; config.legend_y1  = 0.6;
  config.legend_x2  = 0.35; config.legend_y2  = 0.89;
  //config.legend_columns  = 3;
  //config.shift_plot_x  = 0.05;
  config.canvas_ratio  = 0.950;
  config.canvas_grow  = 2;
  //config.rebin_x  = 2;
  //config.normalise  = 0.722*2;

  top_dir="~/comet/160219_BeamBlocker/plots/";
  config.legend_header= "DIO Blocker Height (mm)";
//  file=FixPlot(top_dir+"plot_Momentum-BeamBlocker_240mm.root","THStack", config);
//  SavePlot(ext,NULL,"MomentumVs_DIOBlocker-Beam_240mm");

  config.legend_header= "Beam Blocker Height (mm)";
//  file=FixPlot(top_dir+"plot_Momentum-DIOBlocker_450mm.root","THStack", config);
//  SavePlot(ext,NULL,"MomentumVs_BeamBlocker-DIO_450mm");

  ///////////////////////   integrated Momentum spectra
  config.reset();
  config.line_width =2;
  config.y_axis_label_offset = 1.4;
  config.y_axis_label    = "Rate of Observation (per muon DIO per 4.5 MeV/c)";
  config.legend_header= "Momentum (MeV/c)";
  config.y_axis_decimal  = true;
  config.x_axis_decimal  = true;
  //config.x_axis_range_high  = 120;
  config.legend_text_size= 0.025;
  config.legend_margin= 0.4;
  config.legend_x1  = 0.61 ; config.legend_y1  = 0.7;
  config.legend_x2  = 0.89; config.legend_y2  = 0.89;
  //config.legend_columns  = 3;
  //config.shift_plot_x  = 0.05;
  //config.canvas_ratio  = 1.10;
  config.canvas_ratio  = 0.950;
  config.canvas_grow  = 2;
  //config.rebin_x  = 2;
  //config.normalise  = 0.722*2;


  config.x_axis_label    = "DIO Blocker Height Below Beam Axis (mm)";
//  file=FixPlot(top_dir+"plot_IntegratedMomentum-BeamBlocker_240mm_adjacent.root","TMultiGraph", config);
//  SavePlot(ext,NULL,"IntegratedMomentumVs_DIOBlocker-Beam_240mm");

  config.x_axis_label= "Beam Blocker Radius (mm)";
//  file=FixPlot(top_dir+"plot_IntegratedMomentum-DIOBlocker_450mm_adjacent.root","TMultiGraph", config);
//  SavePlot(ext,NULL,"IntegratedMomentumVs_BeamBlocker-DIO_450mm");

  config.legend_y1  = 0.6;
  config.legend_y2  = 0.79;

  config.x_axis_label    = "DIO Blocker Height Below Beam Axis (mm)";
//  file=FixPlot(top_dir+"plot_IntegratedMomentum-BeamBlocker_240mm_ratios.root","TMultiGraph", config);
//  SavePlot(ext,NULL,"MomentumShapeVs_DIOBlocker-Beam_240mm");

  config.x_axis_label= "Beam Blocker Radius (mm)";
//  file=FixPlot(top_dir+"plot_IntegratedMomentum-DIOBlocker_450mm_ratios.root","TMultiGraph", config);
//  SavePlot(ext,NULL,"MomentumShapeVs_BeamBlocker-DIO_450mm");

  ///////////////////////   Straw Tracker Hits
  config.reset();
  config.line_width =2;
  config.y_axis_label_offset = 1.2;
  //config.shift_plot_x  = 0.05;
  //config.canvas_ratio  = 1.10;
  config.canvas_ratio  = 1;
  config.canvas_grow  = 2;
  config.stats_force_off=true;
  config.log_z=true;

//  file=FixPlot(top_dir+"StrawTracker_HitsXY.root","TH1", config);
//  SavePlot(ext,NULL,"StrawTracker_HitsXY");

//  file=FixPlot(top_dir+"StrawTracker_HitsZvsMom.root","TH1", config);
//  SavePlot(ext,NULL,"StrawTracker_HitsZvsMom");

//  config.y_axis_range_high=110;
//  file=FixPlot(top_dir+"StrawTracker_HitsYvsMom.root","TH1", config);
//  SavePlot(ext,NULL,"StrawTracker_HitsYvsMom");

  ///////////////////////   integrated HitRate
  config.reset();
  config.y_axis_label_offset = 1.6;
  config.y_axis_label    = "Height of Top Edge of DIO Blockers (mm)";
  config.x_axis_label    = "Radius of Beam Blocker (mm)";
  config.z_axis_label    = "Mean Number of Straw Hits per DIO Electron";
  config.z_axis_label_offset=1.3;
  config.z_axis_label_centred=true;
  config.shift_plot_x  = 0.00;
  config.shift_palette_y = -0.05;
  config.canvas_ratio  = 1.05;
  //config.canvas_width  = 1000;
  config.canvas_grow  = 2;
  config.stats_force_off  = true;
  config.pad_margin_right = 0.15;
  config.pad_margin_left  = 0.15;
  config.title    = "Hit Rate Due to DIO Electrons";
  //config.force_draw_option="contztext20";
  //config.log_z=true;

  TGaxis::SetMaxDigits(3);
  gStyle->SetPaintTextFormat("1.1e");

  config.z_axis_range_high=2.45e-3;
  config.marker_color=kGray;
  config.marker_size=1.15;
  top_dir="~/comet/160219_BeamBlocker/DIO_rate/";
  file=FixPlot(top_dir+"160819-files_150-normed_perDIO.root","TH1", config);
  SavePlot(ext,NULL,"HitRate2d");

  config.z_axis_label    = "High-P (>100 MeV/c) Acceptance per DIO Hit";
  config.force_draw_option="colztext30";
  config.marker_color=kGray;
  gStyle->SetHistMinimumZero(false);
  config.z_axis_range_high=2e5;
  config.z_axis_range_low=2.4e2;
  config.log_z=true;
  top_dir="~/comet/160219_BeamBlocker/plots/";
  file=FixPlot(top_dir+"HitRateVsSignalAcceptanceRatio.root","TH1", config);
  SavePlot(ext,NULL,"HitRateVsAcceptance2dRatio");

  config.force_draw_option="";
  config.marker_color=kGray;
  config.log_z=false;
  gStyle->SetHistMinimumZero(true);
  config.marker_color=kBlue;
  config.z_axis_range_high=0;
  config.legend_header="";
  config.legend_x1=0.25; config.legend_x2=0.7; 
  config.legend_y1=0.72;  config.legend_y2=.82;
  top_dir="~/comet/160219_BeamBlocker/plots/";
  file=FixPlot(top_dir+"HitRateVsSignalAcceptanceContours.root","TH1", config);
  SavePlot(ext,NULL,"HitRateVsAcceptance2d");

  return;

  ///////////////////////   Momentum in blocker
  config.reset();
  config.line_width =2;
  config.y_axis_label_offset = 1.4;
  config.y_axis_label    = "Number of hits in Beam Blocker (arb. units)";
  config.x_axis_label    = "Radial Distance from Beam Axis (mm)";
  config.y_axis_decimal  = true;
  config.x_axis_decimal  = true;
  //config.x_axis_range_high  = 120;
  config.x_axis_range_low  = 0;
  config.legend_text_size= 0.025;
  config.legend_margin= 0.4;
  config.legend_header= "Momentum (MeV/c)";
  config.legend_x1  = 0.11 ; config.legend_y1  = 0.6;
  config.legend_x2  = 0.35; config.legend_y2  = 0.89;
  config.legend_columns  = 3;
  config.shift_plot_x  = 0.05;
  config.canvas_ratio  = 1.10;
  config.canvas_grow  = 2;
  config.rebin_x  = 2;
  //config.normalise  = 0.722*2;

//  file=FixPlot(top_dir+"BeamBlockerHits_RadialDistVsMomentumHitContainer-BeamBlocker.root.root","THStack", config);
//  SavePlot(ext,NULL,"MomentumInBeamBlocker");
  cout<<"End"<<endl;
}
