void tidy_plots_cosmics(const char* ext="png"){

  gROOT->ProcessLine(".L ~/thesis/figs/tidy_plot_tools.C+");
  gStyle->SetPalette(55);

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

  TString top_dir;
  top_dir="~/comet/160913_cosmic/160920_Run3/";
  //====================================================
  config.reset();
  //config.z_axis_range_high=1;
  //config.z_axis_range_low=1e-18;
  config.log_y=true;
  config.stats_force_off=true;
  config.x_axis_title_size=config.y_axis_title_size=0.06;
  config.x_axis_label_size=config.y_axis_label_size=0.05;
  config.y_axis_label_offset=1.2;
  config.title=" ";
  config.grid_y=config.grid_x=true;
  config.canvas_ratio=1.1;
  config.shift_plot_x=0.05;
  config.shift_plot_y=-0.05;

  config.legend_x1=0.3; config.legend_x2=0.6;
  config.legend_y1=0.3; config.legend_y2=0.5;
  config.y_axis_range_low=1e-4; config.y_axis_range_high=1;
  config.x_axis_label="Transverse direction cosine, cos(#theta)";
  config.y_axis_label="Probability per bin";
  // Path of high-energy muons
  FixPlot(top_dir+"Primary_transverse_direction.root","TH1", config);
  SavePlot(ext,NULL,"Cosmics_primary_transverse");

  config.legend_x1=0.3; config.legend_x2=0.7;
  config.legend_y1=0.25; config.legend_y2=0.5;
  config.y_axis_range_low=0; config.y_axis_range_high=0;
  config.x_axis_range_low=90; config.x_axis_range_high=1e4;
  config.x_axis_label="Momentum (MeV/c)";
  config.y_axis_label="Binned Probability per {x_bin_widths} MeV/c";
  config.log_x=true;
  // Path of high-energy muons
  FixPlot(top_dir+"Primary_momentum.root","TH1", config);
  SavePlot(ext,NULL,"Cosmics_primary_momentum");

  return;

  //====================================================
  config.reset();
  //config.z_axis_range_high=1;
  //config.z_axis_range_low=1e-18;
  config.x_axis_range_low=16000; config.x_axis_range_high=33000;
  config.y_axis_range_low=-790; config.y_axis_range_high=790;
  config.z_axis_range_low=1; config.z_axis_range_high=50;
  config.log_z=true;
  config.stats_force_off=true;
  config.canvas_ratio=2;
  config.x_axis_title_size=config.y_axis_title_size=0.08;
  config.x_axis_label_size=config.y_axis_label_size=0.07;
  config.z_axis_title_size=0.18;
  config.z_axis_label_size=0.17;
  config.shift_plot_y=-0.08;
  config.shift_palette_y=0.16;
  config.canvas_grow=2;
  config.y_axis_label_offset=0.35;
  config.title=" ";
  config.pad_margin_left=0.1;
  //config.pad_margin_right=0.09;
  config.grid_y=config.grid_x=true;

  config.AddLine(StopTgtSec_start);
  config.AddLine(       EST_start);
  config.AddLine(    DetSol_start);

  // Path of high-energy muons
  DrawFixPlot(top_dir+"combined_analysis-truth_IBeamTagsAnalyzer.root","truth_IBeamTagsAnalyzer/hBeam/hBeam_6","TH1", config,"colz",false);
  if(geomHist) Overlay(geomHist,"same", config);
  SavePlot("png",NULL,"Cosmics_highP_electrons");

  //====================================================
  config.reset();
  //config.z_axis_range_high=1;
  //config.z_axis_range_low=1e-18;
  config.x_axis_range_low=0; config.x_axis_range_high=200;
  //config.y_axis_range_low=-800; config.y_axis_range_high=800;
  //config.canvas_ratio=1.1;
  config.y_axis_label_offset=0.6;
  config.stats_force_off=true;
  config.x_axis_title_size=config.y_axis_title_size=0.06;
  config.x_axis_label_size=config.y_axis_label_size=0.05;
  config.shift_plot_y=-0.05;
  config.title=" ";
  config.x_axis_label="Momentum (MeV/c)";
  config.y_axis_label="Electron Tracks per {x_bin_widths} MeV/c";
  config.rebin_x=8;
  //config.pad_margin_right=0.09;
  config.grid_y=config.grid_x=true;
  config.line_width=2;

  config.legend_header="Number of Straw Tracker Planes Struck";
  config.legend_build_from="legend_obj";
  config.legend_x1=0.43; config.legend_x2=0.84;
  config.legend_y1=0.7; config.legend_y2=0.94;
  config.legend_text_size=0.04;
  config.legend_columns=1;
  config.legend_margin=0.4;

  // Path of high-energy muons
  FixPlot(top_dir+"Overlaid_electron_hits.root","THStack", config);
  SavePlot(ext,NULL,"Cosmics_strawTrackHits");


}
