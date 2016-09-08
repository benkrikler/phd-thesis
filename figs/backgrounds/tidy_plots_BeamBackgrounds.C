void tidy_plots_BeamBackgrounds(const char* ext="png"){

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
  top_dir="~/comet/160807_BeamBackgrounds/160824/";
  //====================================================
  config.reset();
  //config.z_axis_range_high=1;
  //config.z_axis_range_low=1e-18;
  config.x_axis_range_low=14700; config.x_axis_range_high=33000;
  config.y_axis_range_low=-800; config.y_axis_range_high=800;
  config.log_z=true;
  config.stats_force_off=true;
  config.canvas_ratio=2;
  config.x_axis_title_size=config.y_axis_title_size=config.z_axis_title_size=0.06;
  config.x_axis_label_size=config.y_axis_label_size=config.z_axis_label_size=0.05;
  config.shift_plot_y=-0.04;
  config.shift_palette_y=0.08;
  config.canvas_grow=2;
  config.y_axis_label_offset=0.4;
  config.title=" ";
  config.pad_margin_left=0.05;
  //config.pad_margin_right=0.09;

  config.AddLine(StopTgtSec_start);
  config.AddLine(       EST_start);
  config.AddLine(    DetSol_start);
//
//  // Path of high-energy muons
//  DrawFixPlot(top_dir+"complete_analysis-truth_IBeamTagsAnalyzer.root","truth_IBeamTagsAnalyzer/hBeamAll/hBeamAll_11","TH1", config,"colz",false);
//  if(geomHist) Overlay(geomHist,"same", config);
//  SavePlot(ext,NULL,"Beam_Electrons_all");
//
//  config.canvas_grow=1;
//  config.shift_plot_y=0;
//  config.shift_palette_y=0;
//  config.pad_margin_left=0;
//
//  DrawFixPlot(top_dir+"complete_analysis-truth_IBeamTagsAnalyzer.root","truth_IBeamTagsAnalyzer/hBeam/hBeam_3","TH1", config,"colz",false);
//  if(geomHist) Overlay(geomHist,"same", config);
//  SavePlot(ext,NULL,"Beam_Muons_dangerous");
//

  //====================================================
  // Timing and momentum of particles at the Straw Tracker (with legend)
  // Timing and momentum of particles at the stopping target (before it)
  config.reset();
  //config.y_axis_range_low=-800; config.y_axis_range_high=800;
  config.stats_force_off=true;
  //config.canvas_ratio=2;
  config.x_axis_title_size=config.y_axis_title_size=0.06;
  config.x_axis_label_size=config.y_axis_label_size=0.05;
  config.line_width=2;
  config.log_y=true;
  config.grid_y=config.grid_x=true;

  config.title=" ";
  //config.pad_margin_right=0.09;

  config.legend_build_from="legend_obj";
  config.legend_x1=0.49; config.legend_x2=0.89; 
  config.legend_y1=0.71; config.legend_y2=0.89; 
  config.legend_columns=2;
  config.legend_margin=0.4;
  config.stack_remove="triton;alpha;pi+;deuteron;dynamic;e+;mu+;proton;neutron";

  config.x_axis_range_low=0; config.x_axis_range_high=140;
  // Momentum
  DrawFixPlot(top_dir+"complete_analysis-truth_IPhaseIISignalAcceptanceAnalyzer.root","truth_IPhaseIISignalAcceptanceAnalyzer/StrawTrk/hMomentum_StrawTrkCombined","THStack", config,"nostack");
  SavePlot(ext,NULL,"Beam_StrawTrk_Momentum");
  DrawFixPlot(top_dir+"complete_analysis-truth_IPhaseIISignalAcceptanceAnalyzer.root","truth_IPhaseIISignalAcceptanceAnalyzer/Torus2Monitor_1/hMomentum_Torus2Monitor_1Combined","THStack", config,"nostack");
  SavePlot(ext,NULL,"Beam_Torus2Monitor_1_Momentum");
  DrawFixPlot(top_dir+"complete_analysis-truth_IPhaseIISignalAcceptanceAnalyzer.root","truth_IPhaseIISignalAcceptanceAnalyzer/StopTgtSecMonitor_1/hMomentum_StopTgtSecMonitor_1Combined","THStack", config,"nostack");
  SavePlot(ext,NULL,"Beam_StopTgtSecMonitor_1_Momentum");

  config.x_axis_range_low=0; config.x_axis_range_high=2000;
  config.rebin_x=2;
  // Timing
  DrawFixPlot(top_dir+"complete_analysis-truth_IPhaseIISignalAcceptanceAnalyzer.root","truth_IPhaseIISignalAcceptanceAnalyzer/StrawTrk/hTime_StrawTrkCombined","THStack", config,"nostack");
  SavePlot(ext,NULL,"Beam_StrawTrk_Time");
  DrawFixPlot(top_dir+"complete_analysis-truth_IPhaseIISignalAcceptanceAnalyzer.root","truth_IPhaseIISignalAcceptanceAnalyzer/Torus2Monitor_1/hTime_Torus2Monitor_1Combined","THStack", config,"nostack");
  SavePlot(ext,NULL,"Beam_Torus2Monitor_1_Time");
  DrawFixPlot(top_dir+"complete_analysis-truth_IPhaseIISignalAcceptanceAnalyzer.root","truth_IPhaseIISignalAcceptanceAnalyzer/StopTgtSecMonitor_1/hTime_StopTgtSecMonitor_1Combined","THStack", config,"nostack");
  SavePlot(ext,NULL,"Beam_StopTgtSecMonitor_1_Time");
}
