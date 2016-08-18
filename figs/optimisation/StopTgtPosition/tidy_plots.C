void tidy_plots(const char* ext="png"){

  gROOT->ProcessLine(".L ~/thesis/figs/tidy_plot_tools.C+");

  PlotConfig config;

  TFile* geomFile=TFile::Open("../Geometry-on-axis.root","READ");
  TH1* geomHist=NULL;
  if(geomFile){
	  geomHist=(TH1*)geomFile->Get("hGeometry");
  }

  TGaxis::SetMaxDigits(3);
  ///////////////////////   Stopping Rate
  config.reset();
  config.y_axis_label_offset = 0.8;
  config.y_axis_label    = "Muon Stopping Rate per POT";
  config.x_axis_label    = "Target Shift (cm)";
  config.y_axis_decimal  = true;
  config.y_axis_title_size=0.05;
  config.y_axis_label_size=0.05;
  config.x_axis_title_size=0.05;
  config.x_axis_label_size=0.05;
  //config.x_axis_range_high  = 120;
  config.x_axis_range_low  = 0;
  config.y_axis_range_low  = 1.3e-3;
  config.y_axis_range_high  = 2.3e-3;
  config.legend_text_size= 0.022;
  config.legend_x1  = 0.5 ; config.legend_y1  = 0.6;
  config.legend_x2  = 0.95; config.legend_y2  = 0.85;
  //config.shift_plot_x  = 0.05;
  config.line_width=3;
  config.grid_y=config.grid_x=true;
  config.canvas_ratio=1.4;
  config.shift_plot_y=-0.01;
  config.title=" ";

  TString top_dir="~/comet/1601w04_StopTarget_DIO_optimisation/";
  //file=FixPlot(top_dir+"StoppingRate_with-without_beamBlocker.root","", config);
//  file=FixPlot(top_dir+"160220_StoppingRate_muons-start_-50.root","TGraph", config);
//  SavePlot(ext,NULL,"Tidied_MuonStoppingRate");

  ///////////////////////   Accepted momentum
  config.reset();
  config.y_axis_label_offset = 0.9;
  config.y_axis_title_size=0.04;
  config.y_axis_label    = "#splitline{Electron acceptance (per {x_bin_widths} MeV/c)}{#times Muon Stopping Rate per POT}";
  config.x_axis_label    = "Electron Momentum at Stopping Target (MeV/c)";
  config.y_axis_decimal  = true;
  config.x_axis_range_high  = 120;
  config.x_axis_range_low  = 0;
  config.legend_text_size= 0.040;
  config.legend_header= "Target Shift (cm)";
  config.legend_x1  = 0.13 ; config.legend_y1  = 0.4;
  config.legend_x2  = 0.40; config.legend_y2  = 0.85;
  config.shift_plot_x  = 0.05;
  config.legend_margin=0.5;
  config.grid_y=config.grid_x=true;
  config.line_width=2;
  //config.force_draw_option="L";
  config.canvas_ratio=1.4;

  config.rebin_x=8;
//  file=FixPlot(top_dir+"no_beam_blocked_analysis/160816_remade_plots/NoBeamBlocker-AcceptedMomentum.root","THStack", config);
//  SavePlot(ext);

  config.rebin_x=2;
  TString top_dir="~/comet/160213_StopTgtPositionOptimisation/";
//  file=FixPlot(top_dir+"160816_plots/-AcceptedMomentum.root","THStack", config);
//  SavePlot(ext);

  config.canvas_ratio=0.8;
  config.legend_text_size= 0.035;
  config.y_axis_label    = "#splitline{Electron acceptance (per 5 MeV/c)}{#times Muon Stopping Rate per POT}";
  config.x_axis_label    = "Target Shift (cm)";
  config.legend_header= "Electron Momentum at Stopping Target (MeV/c)";
  config.legend_x1  = 0.13 ; config.legend_y1  = 0.75;
  config.legend_x2  = 0.86; config.legend_y2  = 0.89;
  config.legend_margin=0.6;
  config.legend_columns=6;
  config.rebin_x=1;
  config.x_axis_label_size=0.04;
  config.x_axis_title_size=0.045;
  config.x_axis_label_offset = 0.8;
  config.y_axis_range_high=11.5e-3;
  config.x_axis_range_high=50;
  config.x_axis_range_low=-50;
  config.y_axis_label_offset = 1.3;
  file=FixPlot(top_dir+"160816_plots/-AcceptedMomentum-Integrated_adjacent.root","TMultiGraph", config);
  SavePlot(ext);

  config.y_axis_label_offset = 1.2;
  config.y_axis_label    = "Relative Acceptance for Electrons";
  config.y_axis_range_high=1.25;
//  file=FixPlot(top_dir+"160816_plots/-AcceptedMomentum-Integrated_ratios.root","TMultiGraph", config);
//  SavePlot(ext);

  ///////////////////////   Integrated momentum acceptance
  config.reset();
  config.y_axis_label_offset = 1.9;
  config.x_axis_label    = "Target Shift (cm)";
  config.y_axis_decimal  = true;
  config.y_axis_range_high  = 0.018;
  config.x_axis_range_high  = 100;
  config.x_axis_range_low  = 0;
  config.legend_text_size= 0.022;
  config.legend_header= "Momentum (MeV/c)";
  config.legend_x1  = 0.13 ; config.legend_y1  = 0.8;
  config.legend_x2  = 0.87; config.legend_y2  = 0.89;
  config.legend_columns=7;
  config.shift_plot_x  = 0.05;
  config.line_width  = 3;
  config.canvas_grow  = 2;

  config.y_axis_label    = "Electron acceptance #times muon stopping rate per POT";
//  file=FixPlot(top_dir+"160216_plots/AcceptedMomentum-Integrated_adjacent.root","TMultiGraph", config);
//  SavePlot("WithBeamBlocker-BinnedAcceptanceVsTgtShift","png");

//  ///////////////////////   Flux 
//  config.reset();
//  file=FixPlot(top_dir+"no_beam_blocked_analysis/Flux-IntrinsicElectrons-Shift_040cm.root","THStack", config);
//  SavePlot("NoBeamBlocker-Flux-Shift_040cm","png");
//
//  file=FixPlot(top_dir+"scan_analysis/Flux-IntrinsicElectrons-Shift_040cm.root","THStack", config);
//  SavePlot("WithBeamBlocker-Flux-Shift_040cm","png");

  ///////////////////////   Phi accepted
  config.reset();
  config.line_width  = 3;
  config.shift_plot_x = 0.05;
  //config.x_axis_label    = "Phi Angle (^{#circ})";
  //config.canvas_width = 900;
  config.canvas_grow  = 2;
  config.y_axis_decimal  = true;
  config.y_axis_label_offset  = 1;
  config.legend_columns=5;
  config.legend_text_size= 0.022;
  config.legend_header= "Momentum (MeV/c)";
  config.legend_x1  = 0.13 ; config.legend_y1  = 0.76;
  config.legend_x2  = 0.87; config.legend_y2  = 0.89;

//  file=FixPlot(top_dir+"no_beam_blocked_analysis/PhiAccepted-IntrinsicElectrons-Shift_040cm.root","THStack", config);
//  SavePlot("NoBeamBlocker-PhiAccepted-Shift_040cm","png");

  config.y_axis_range_high  = 0.8;
  config.y_axis_range_low  = 0;
//  file=FixPlot(top_dir+"160221_plots/-PhiAcceptanceNormalised_050.root","THStack", config);
//  SavePlot("WithBeamBlocker-PhiAccepted-Shift_050cm","png");

  config.y_axis_range_high  = 0.65;
  config.canvas_ratio  = 1.3; 
  config.legend_remove  = true; 
//  for(int i=0; i <=100; i+=10){
//	  file=FixPlot(top_dir+Form("160221_plots/-PhiAcceptanceNormalised_%03d.root",i),"THStack", config);
//	  SavePlot(Form("WithBeamBlocker-PhiAccepted-Shift_%03dcm-squeezed",i),"png");
//  }

  ///////////////////////   Theta accepted
//  file=FixPlot(top_dir+"no_beam_blocked_analysis/ThetaAccepted-IntrinsicElectrons-Shift_040cm.root","THStack", config);
//  SavePlot("NoBeamBlocker-ThetaAccepted-Shift_040cm","png");

  config.y_axis_range_high  = 1.;
  config.canvas_ratio  = 1; 
//  file=FixPlot(top_dir+"160221_plots/-PhiAcceptanceNormalised_050.root","THStack", config);
//  SavePlot("WithBeamBlocker-PhiAccepted-Shift_050cm","png");

  config.y_axis_range_high  = 1.1;
  config.canvas_ratio  = 1.3; 
  config.legend_remove  = true; 
//  for(int i=0; i <=100; i+=10){
//	  file=FixPlot(top_dir+Form("160221_plots/-ThetaAcceptanceNormalised_%03d.root",i),"THStack", config);
//	  SavePlot(Form("WithBeamBlocker-ThetaAccepted-Shift_%03dcm-squeezed",i),"png");
//  }


//  ///////////////////////   Heights
//  config.reset();
//  config.y_axis_range_high=200;
//  config.y_axis_range_low=-800;
//  file=FixPlot(top_dir+"no_beam_blocked_analysis/HeightOverlay-IntrinsicElectrons-Shift_040cm.root","THStack", config);
//  SavePlot("NoBeamBlocker-HeightOverlay-Shift_040cm","png");
//
//  file=FixPlot(top_dir+"scan_analysis/HeightOverlay-IntrinsicElectrons-Shift_040cm.root","THStack", config);
//  SavePlot("WithBeamBlocker-HeightOverlay-Shift_050cm","png");

  ///////////////////////   Heights for different target positions
  config.reset();
  config.x_axis_range_low=16000;
  config.x_axis_range_high=33000;
  config.line_width  = 1.5;
  //config.shift_plot_x = 0.05;
  //config.x_axis_label    = "Phi Angle (^{#circ})";
  //config.canvas_width = 900;
  config.canvas_width=1500;
  config.canvas_height=800;
  config.y_axis_decimal  = true;
  config.y_axis_label_offset  = 1;
  config.legend_columns=2;
  config.legend_header= "Target Position (cm)";
  config.legend_text_size= 0.022;
  config.legend_x1  = 0.13 ; config.legend_y1  = 0.6;
  config.legend_x2  = 0.4; config.legend_y2  = 0.89;

  AnnotatedLine StopTgtSec_start("",18000, plot::kMin, 18000,plot::kMax);StopTgtSec_start.SetLineAppearance(kMagenta,2);
  AnnotatedLine        EST_start("",21700, plot::kMin, 21700,plot::kMax);       EST_start.SetLineAppearance(kMagenta,2); 
  AnnotatedLine     DetSol_start("",28000, plot::kMin, 28000,plot::kMax);    DetSol_start.SetLineAppearance(kMagenta,2);
  config.AddLine(StopTgtSec_start);
  config.AddLine(EST_start);
  config.AddLine(DetSol_start);

  config.legend_text_size= 0.05;
  config.legend_x1  = 0.9 ; config.legend_y1  = 0.16;
  config.legend_x2  = 0.98; config.legend_y2  = 0.96;
  config.legend_columns=1;
  config.legend_header= "Target Pos.(cm)";
  config.y_axis_label_size= 0.05;
  config.x_axis_label_size= 0.05;
  config.x_axis_label="Distance Along Beam Axis (m)";
  config.x_axis_title_size=0.06;
  config.y_axis_title_size=0.06;
  config.y_axis_label_offset=0.4;
  config.shift_plot_y=-0.03;
  config.grid_y=config.grid_x=true;
  config.title=" ";
  config.y_axis_label="Height Above Beam Axis (mm)";

  config.canvas_height=300;
  config.y_axis_range_high=100;
  config.y_axis_range_low=-800;
  file=FixPlot(top_dir+"160816_plots/-Height-momentum_102.5.root","THStack", config);
  if(geomHist) Overlay(geomHist,"same",config);
  SavePlot(ext,NULL,"WithBeamBlocker-Height-VaryShifts-Momentum_102-5");

  file=FixPlot(top_dir+"160816_plots/-Height-momentum_82.5.root","THStack", config);
  if(geomHist) Overlay(geomHist,"same",config);
  SavePlot(ext,NULL,"WithBeamBlocker-Height-VaryShifts-Momentum_82-5");

  file=FixPlot(top_dir+"160816_plots/-Height-momentum_62.5.root","THStack", config);
  if(geomHist) Overlay(geomHist,"same",config);
  SavePlot(ext,NULL,"WithBeamBlocker-Height-VaryShifts-Momentum_62-5");

  file=FixPlot(top_dir+"160816_plots/-Height-momentum_42.5.root","THStack", config);
  if(geomHist) Overlay(geomHist,"same",config);
  SavePlot(ext,NULL,"WithBeamBlocker-Height-VaryShifts-Momentum_42-5");

  config.y_axis_range_high=150;
  config.y_axis_range_low=-100;
  file=FixPlot(top_dir+"160816_plots/-Height-momentum_102.5.root","THStack", config);
  //if(geomHist) Overlay(geomHist,"same",config);
  SavePlot(ext,NULL,"WithBeamBlocker-Height-VaryShifts-Momentum_102-5-Zoom");
}
