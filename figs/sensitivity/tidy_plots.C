void tidy_plots(const char* ext="png"){

  gROOT->ProcessLine(".L ~/thesis/figs/tidy_plot_tools.C+");
  gStyle->SetPalette(55);

  TString top_dir="~/comet/1603w12_Phase-II_estimates/160328_signal_acceptance/";
  //TString top_dir=gSystem->ExpandPathName("$PWD/");
  PlotConfig config;

  TFile* geomFile=TFile::Open("../Geometry-on-axis_Optimised.root","READ");
  TH1* geomHist=NULL;
  if(geomFile){
	  geomHist=(TH1*)geomFile->Get("hGeometry");
  }

//  gStyle->SetNumberContours(254);
  gStyle->SetStripDecimals(false);
  gStyle->SetPaintTextFormat(".2f");
  gStyle->SetPalette(55);

  AnnotatedLine StopTgtSec_start("", 16700, plot::kMin, 16700, plot::kMax); StopTgtSec_start.SetLineAppearance(kMagenta, 1 ,2);
  AnnotatedLine        EST_start("", 21800, plot::kMin, 21800, plot::kMax); EST_start.SetLineAppearance       (kMagenta, 2 ,2);
  AnnotatedLine     DetSol_start("", 28000, plot::kMin, 28000, plot::kMax); DetSol_start.SetLineAppearance    (kMagenta, 2 ,2);

  //====================================================
  //config.y_axis_label_offset = 1.3;
  config.y_axis_decimal  = true;
  config.x_axis_range_high  = 33e3;
  //config.y_axis_range_high  = 110;
  //config.y_axis_label  = "Observed Momentum (MeV/c)";
  //config.x_axis_label  = "Initial Momentum (MeV/c)";
  config.log_y=false; config.log_z=true;
  config.stats_force_off=true;
  config.grid_y=config.grid_x=true;
  config.force_draw_option="col";

  file=FixPlot(top_dir+"160417_plot_SignalElectronAcceptanceVsBeamline.root","TH1", config);
  SavePlot(ext,NULL,"Tidied_SignalSurivivalVsBeamline");

  file=FixPlot(top_dir+"160417_plot_SignalElectron_HeightVsBeamline.root","TH1", config);
  if(geomHist) Overlay(geomHist,"same",config);
  SavePlot(ext,NULL,"Tidied_SignalHeight2DVsBeamline");

  return;

  //====================================================
  //config.y_axis_label_offset = 1.3;
  config.y_axis_decimal  = true;
  config.x_axis_range_high  = 110;
  config.y_axis_range_high  = 110;
  config.y_axis_label  = "Observed Momentum (MeV/c)";
  config.x_axis_label  = "Initial Momentum (MeV/c)";
  config.log_y=false; config.log_z=true;

  file=FixPlot(top_dir+"MomentumTransfer-Hit_StrawTracker.root","TH1", config);
  SavePlot(ext,NULL,"Tidied_MomentumTransfer-Hit_StrawTracker");

  file=FixPlot(top_dir+"MomentumTransfer-realisticAccept.root","TH1", config);
  SavePlot(ext,NULL,"Tidied_MomentumTransfer-realisticAccept");

  //config.reset();
  //config.legend_columns=5;
  //config.legend_margin=0.5;
  //config.legend_text_size= 0.024;
  //config.legend_x1  = 0.45; config.legend_y1  = 0.75;
  //config.legend_x2  = 0.88; config.legend_y2  = 0.88;
}
