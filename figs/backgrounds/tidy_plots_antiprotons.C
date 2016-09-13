void tidy_plots_antiprotons(const TString ext="png"){

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

  AnnotatedLine          ProdTgt("",  1850, plot::kMin,  1850, plot::kMax); ProdTgt.SetLineAppearance         (kMagenta, 2 ,2);
  AnnotatedLine     Torus1_start("",  6020, plot::kMin,  6020, plot::kMax); Torus1_start.SetLineAppearance    (kMagenta, 2 ,2);
  AnnotatedLine StopTgtSec_start("", 16700, plot::kMin, 16700, plot::kMax); StopTgtSec_start.SetLineAppearance(kMagenta, 2 ,2);
  AnnotatedLine        EST_start("", 21800, plot::kMin, 21800, plot::kMax); EST_start.SetLineAppearance       (kMagenta, 2 ,2);
  AnnotatedLine     DetSol_start("", 28000, plot::kMin, 28000, plot::kMax); DetSol_start.SetLineAppearance    (kMagenta, 2 ,2);

  int angles[4]={10,59,97,119};

  TString top_dir;
  top_dir="~/comet/160501_AntiProtonSpectrum/160907_FixThings/simulation_results/";
  //==================================================== Momentum
  config.reset();
  //config.z_axis_range_high=1;
  //config.z_axis_range_low=1e-18;
  //config.x_axis_range_low=14700; config.x_axis_range_high=33000;
  config.stats_force_off=true;
  config.canvas_ratio=1.4;
  config.x_axis_title_size=config.y_axis_title_size=0.06;
  config.x_axis_label_size=config.y_axis_label_size=0.05;
  //config.shift_plot_y=-0.04;
  //config.shift_palette_y=0.08;
  config.canvas_grow=2;
  config.y_axis_label_offset=0.8;
  config.title=" ";
  config.pad_margin_left=0.15;
  //config.pad_margin_right=0.09;
  config.line_width=2;
  config.line_style=9;
  config.grid_y=config.grid_x=true;
  config.x_axis_divisions=505;
  gStyle->SetNdivisions(505);
  config.shift_plot_x=0.05;
  config.shift_plot_y=-0.03;
  config.title=" ";

  config.legend_x1=0.18; config.legend_x2=0.89; 
  config.legend_y1=0.85; config.legend_y2=0.955; 
  config.legend_margin=0.5;
  //config.legend_text_size=0.025;
  config.legend_columns=5;
  config.legend_header="Antiproton Production Angle (degrees)";

  config.x_axis_label="Momentum (MeV/c)";
  config.rebin_x=8;
  config.x_axis_range_low=0; config.x_axis_range_high=220;
  config.y_axis_range_low=0; config.y_axis_range_high=6.5e-5;
  //config.log_y=true;
  config.y_axis_label="Mean Flux per POT per {x_bin_widths} MeV/c  ";
  //config.force_draw_option="Lnostack";
  FixPlot(top_dir+"pionMomentum_wPiFromPOT.root","THStack", config);
  SavePlot(ext,NULL,"Antiproton_pion_momentum");

  config.legend_columns=2;
  config.legend_x1=0.52; config.legend_x2=0.89; 
  config.legend_y1=0.75; config.legend_y2=0.955; 
  config.x_axis_label="Arrival Time (ns)";
  config.rebin_x=4;
  config.x_axis_range_low=0; config.x_axis_range_high=300;
  config.y_axis_range_low=0; config.y_axis_range_high=0;
  config.log_y=true;
  config.y_axis_label="Mean Flux per POT per {x_bin_widths} ns ";
  //config.force_draw_option="Lnostack";
  FixPlot(top_dir+"pionTimeTor1Entr.root","THStack", config);
  SavePlot(ext,NULL,"Antiproton_pion_time");
  FixPlot(top_dir+"pionTimeTor2Entr.root","THStack", config);
  SavePlot(ext,NULL,"Antiproton_pion_time_tor2");


  return;

  //====================================================
  config.reset();
  //config.z_axis_range_high=1;
  //config.z_axis_range_low=1e-18;
  //config.x_axis_range_low=14700; config.x_axis_range_high=33000;
  config.stats_force_off=true;
  config.canvas_ratio=2;
  config.x_axis_title_size=config.y_axis_title_size=0.06;
  config.x_axis_label_size=config.y_axis_label_size=0.05;
  config.x_axis_label="Distance Along Beam Axis (mm)";
  config.shift_plot_y=-0.04;
  config.shift_palette_y=0.08;
  config.canvas_grow=2;
  config.y_axis_label_offset=0.5;
  config.title=" ";
  config.pad_margin_left=0.1;
  //config.pad_margin_right=0.09;
  config.line_width=2;
  config.grid_y=config.grid_x=true;
  config.log_z=true;
  config.x_axis_range_low=0; config.x_axis_range_high=20500;

  config.AddLine(ProdTgt);
  config.AddLine(Torus1_start);
  config.AddLine(StopTgtSec_start);
  config.AddLine(       EST_start);
  config.AddLine(    DetSol_start);

  config.legend_build_from="legend_obj";
  config.legend_x1=0.30; config.legend_x2=0.89; 
  config.legend_y1=0.80; config.legend_y2=0.95; 
  config.legend_columns=8;
  config.legend_margin=0.5;
  //config.stack_remove="antineutron;triton;alpha;deuteron;dynamic;neutron";
  config.stack_include="mu+;mu-;pi+;pi-;neutron;gamma;e+;e-;antiproton;proton";

  //      gStyle->SetOptLogz(true);

  // Fluxes
  for(int i=0;i<4;++i){
      config.y_axis_range_low=1e-6; config.y_axis_range_high=10;
      config.log_y=true;
      config.y_axis_label="Flux per initial antiproton   ";
      config.force_draw_option="nostack";
      DrawFixPlot(top_dir+Form("Angle_%d-combined_analysis_truth_IPhaseIISignalAcceptanceAnalyzer.root",angles[i]),"truth_IPhaseIISignalAcceptanceAnalyzer/hBeamlineFluxEndsAllCombined","THStack", config,"nostack");
      SavePlot(ext,NULL,Form("Antiproton_flux_%d",angles[i]));

        config.legend_y1=config.legend_y1+config.shift_plot_y; 
        config.legend_y2=config.legend_y2+config.shift_plot_y; 
        config.canvas_grow=1;
        config.shift_plot_y=0;
        config.shift_palette_y=0;
        config.pad_margin_left=0;


        if(ext=="pdf")continue;

      config.force_draw_option="colz";
      config.y_axis_label="Height Above Beam Axis (mm)   ";
      config.y_axis_range_low=-800; config.y_axis_range_high=800;
      config.z_axis_range_low=2e-6; config.z_axis_range_high=0.05;
      config.log_y=false;
      DrawFixPlot(top_dir+Form("Angle_%d-combined_analysis_truth_IPhaseIISignalAcceptanceAnalyzer.root",angles[i]),"truth_IPhaseIISignalAcceptanceAnalyzer/hBeamHeightAll/hBeamHeightAll_-2212","TH1", config,"colz",false);
      if(geomHist) Overlay(geomHist,"same", config);
      SavePlot(ext,NULL,Form("Antiproton_height2D_antiproton_%d",angles[i]));

      DrawFixPlot(top_dir+Form("Angle_%d-combined_analysis_truth_IPhaseIISignalAcceptanceAnalyzer.root",angles[i]),"truth_IPhaseIISignalAcceptanceAnalyzer/hBeamHeightAll/hBeamHeightAll_-211","TH1", config,"colz",false);
      if(geomHist) Overlay(geomHist,"same", config);
      SavePlot(ext,NULL,Form("Antiproton_height2D_pi-_%d",angles[i]));

      DrawFixPlot(top_dir+Form("Angle_%d-combined_analysis_truth_IPhaseIISignalAcceptanceAnalyzer.root",angles[i]),"truth_IPhaseIISignalAcceptanceAnalyzer/hBeamHeightAll/hBeamHeightAll_13","TH1", config,"colz",false);
      if(geomHist) Overlay(geomHist,"same", config);
      SavePlot(ext,NULL,Form("Antiproton_height2D_mu-_%d",angles[i]));
  }

return;

  //====================================================
  config.reset();
  //config.z_axis_range_high=1;
  //config.z_axis_range_low=1e-18;
  config.x_axis_range_low=0; config.x_axis_range_high=160;
  //config.y_axis_range_low=1e-6; config.y_axis_range_high=10;
  config.log_y=true;
  config.stats_force_off=true;
  config.canvas_ratio=0.8;
  config.x_axis_title_size=config.y_axis_title_size=0.06;
  config.x_axis_label_size=config.y_axis_label_size=0.05;
  config.x_axis_label="Momentum (MeV/c)";
  config.y_axis_label="Flux per initial antiproton";
  config.shift_plot_y=-0.04;
  config.shift_palette_y=0.08;
  config.canvas_grow=2;
  config.y_axis_label_offset=1.2;
  config.title=" ";
  config.pad_margin_left=0.2;
  //config.pad_margin_right=0.09;
  config.force_draw_option="nostack";
  config.line_width=2;
  config.grid_y=config.grid_x=true;

//  config.legend_build_from="legend_obj";
//  config.legend_x1=0.16; config.legend_x2=0.89; 
//  config.legend_y1=0.84; config.legend_y2=0.95; 
//  config.legend_columns=8;
//  config.legend_margin=0.5;
//  //config.stack_remove="antineutron;triton;alpha;deuteron;dynamic;neutron";
//  config.stack_include="mu+;mu-;pi+;pi-;neutron;gamma;e+;e-;antiproton;proton";

  // Fluxes
  for(int i=0;i<4;++i){
    if(i>0){
        config.legend_y1=config.legend_y1+config.shift_plot_y; 
        config.legend_y2=config.legend_y2+config.shift_plot_y; 
        config.canvas_grow=1;
        config.shift_plot_y=0;
        config.shift_palette_y=0;
        config.pad_margin_left=0;
    }
    DrawFixPlot(top_dir+Form("Angle_%d-combined_analysis_truth_IPhaseIISignalAcceptanceAnalyzer.root",angles[i]),"truth_IPhaseIISignalAcceptanceAnalyzer/Torus2Monitor_1/hMomentum_Torus2Monitor_1Combined","THStack", config,"nostack");
    SavePlot(ext,NULL,Form("Antiproton_simMomentum_Torus2Mon1_%d",angles[i]));
    }

  // Fluxes
  for(int i=0;i<4;++i){
    if(i>0){
        config.legend_y1=config.legend_y1+config.shift_plot_y; 
        config.legend_y2=config.legend_y2+config.shift_plot_y; 
        config.canvas_grow=1;
        config.shift_plot_y=0;
        config.shift_palette_y=0;
        config.pad_margin_left=0;
    }
    DrawFixPlot(top_dir+Form("Angle_%d-combined_analysis_truth_IPhaseIISignalAcceptanceAnalyzer.root",angles[i]),"truth_IPhaseIISignalAcceptanceAnalyzer/StopTgtSecMonitor_1/hMomentum_StopTgtSecMonitor_1Combined","THStack", config,"nostack");
    SavePlot(ext,NULL,Form("Antiproton_simMomentum_StopTgtSec_%d",angles[i]));
  }
}
