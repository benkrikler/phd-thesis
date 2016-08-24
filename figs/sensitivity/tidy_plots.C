void tidy_plots(const char* ext="png"){

  gROOT->ProcessLine(".L ~/thesis/figs/tidy_plot_tools.C+");
  gStyle->SetPalette(55);

  TString top_dir;
  //TString top_dir=gSystem->ExpandPathName("$PWD/");
  PlotConfig config;

  TFile* geomFile=TFile::Open("../Geometry-on-axis_Optimised.root","READ");
  TH1* geomHist=NULL;
  if(geomFile){
	  geomHist=(TH1*)geomFile->Get("hGeometry");
  }

  //TGaxis::SetMaxDigits(2);
  gStyle->SetNumberContours(255);
  gStyle->SetStripDecimals(false);
  gStyle->SetPaintTextFormat(".2f");
  gStyle->SetPalette(55);

  AnnotatedLine StopTgtSec_start("", 16700, plot::kMin, 16700, plot::kMax); StopTgtSec_start.SetLineAppearance(kMagenta, 1 ,2);
  AnnotatedLine        EST_start("", 21800, plot::kMin, 21800, plot::kMax); EST_start.SetLineAppearance       (kMagenta, 2 ,2);
  AnnotatedLine     DetSol_start("", 28000, plot::kMin, 28000, plot::kMax); DetSol_start.SetLineAppearance    (kMagenta, 2 ,2);

//  //====================================================
//  config.y_axis_label_offset = 1.35;
//  config.y_axis_decimal  = true;
//  //config.x_axis_range_high  = 33e3;
//  //config.y_axis_range_high  = 110;
//  //config.y_axis_label  = "Observed Momentum (MeV/c)";
//  //config.x_axis_label  = "Initial Momentum (MeV/c)";
//  config.log_y=false; config.log_z=false;
//  config.stats_force_off=true;
//  config.legend_remove=true;
//  config.grid_y=config.grid_x=true;
//  config.force_draw_option="colz";
//  config.canvas_ratio=1.0001;
//  config.normalise=1119377985.;
//  config.title  = " ";
//
//  top_dir="~/comet/160807_BeamBackgrounds/160824_plots/G4Trajectories_IStoppedInTgtAnalyzer/";
//
//  // === 2D plots ===
//  config.z_axis_label="#mu stops per POT per {x_bin_widths} #times{y_bin_widths} mm^{2}";
//  config.z_axis_label_centred=true;
//  config.z_axis_label_offset=1.4;
//  config.shift_plot_x=-0.03;
//
//  config.y_axis_range_low=-(config.y_axis_range_high=110);
//  file=FixPlot(top_dir+"hPosition3D/hPosition3D_13XY_lin.root","TH1", config);
//  SavePlot(ext,NULL,"MuStops_2D_XY");
//
//  config.rebin_x=5;
//  config.y_axis_range_low=-(config.y_axis_range_high=110);
//  config.x_axis_range_low=1000; config.x_axis_range_high=1900;
//  file=FixPlot(top_dir+"hPosition3D/hPosition3D_13ZY_log.root","TH1", config);
//  SavePlot(ext,NULL,"MuStops_2D_ZY");
//
//  config.y_axis_label_offset = 1.55;
//  config.rebin_y=config.rebin_x;
//  config.rebin_x=1;
//  //config.y_axis_range_low=-(config.y_axis_range_high=110);
//  config.x_axis_range_low=0; config.x_axis_range_high=-1;
//  config.y_axis_range_low=1000; config.y_axis_range_high=1900;
//  file=FixPlot(top_dir+"hPosition3D/hPosition3D_13XZ_lin.root","TH1", config);
//  SavePlot(ext,NULL,"MuStops_2D_XZ");
//
//  //  === 1D plots ===
//  config.shift_plot_x=-0.05;
//  config.y_axis_label_offset = 1.35;
//  config.y_axis_range_low=-(config.y_axis_range_high=0);
//  config.y_axis_label="Muon Stopping Rate per POT per {x_bin_widths} mm";
//  config.rebin_x=1;
//  config.shift_plot_x=0;
//
//  //config.x_axis_range_low=-(config.x_axis_range_high=110);
//  file=FixPlot(top_dir+"hPositionX/hPositionX_13_lin.root","TH1", config);
//  SavePlot(ext,NULL,"MuStops_1D_X");
//
//  config.x_axis_range_low=-(config.x_axis_range_high=110);
//  file=FixPlot(top_dir+"hPositionY/hPositionY_13_lin.root","TH1", config);
//  SavePlot(ext,NULL,"MuStops_1D_Y");
//
//  config.y_axis_label_offset = 1.55;
//  config.rebin_x=5;
//  config.x_axis_range_low=1000; config.x_axis_range_high=1900;
//  file=FixPlot(top_dir+"hPositionZ/hPositionZ_13_lin.root","TH1", config);
//  SavePlot(ext,NULL,"MuStops_1D_Z");

  //====================================================
  config.reset()
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
  config.canvas_grow=3;

  top_dir="~/comet/1603w12_Phase-II_estimates/160328_signal_acceptance/";
  file=FixPlot(top_dir+"160417_plot_SignalElectronAcceptanceVsBeamline.root","TH1", config);
  SavePlot(ext,NULL,"Tidied_SignalSurivivalVsBeamline");

  file=FixPlot(top_dir+"160417_plot_SignalElectron_HeightVsBeamline.root","TH1", config);
  if(geomHist) Overlay(geomHist,"same",config);
  SavePlot("png",NULL,"Tidied_SignalHeight2DVsBeamline");

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
