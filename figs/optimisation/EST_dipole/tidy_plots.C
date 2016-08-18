void tidy_plots(const char* ext="png"){

  gROOT->ProcessLine(".L ~/thesis/figs/tidy_plot_tools.C+");
  // gStyle->SetStripDecimals(false);
  gStyle->SetPalette(55);
  gStyle->SetNumberContours(255);
 // gStyle->SetPaintTextFormat("0.3e");
//  gStyle->SetPadRightMargin(0.5);
//  gStyle->SetTextColor(kWhite);

  TString top_dir="~/comet/1602w06_EST_Dipole/";
  PlotConfig config;

  AnnotatedLine       Tor1_start ( "" , 16700-4.7e3*2-800 , plot::kMin , 16700-4.7e3*2-800 , plot::kMax ); Tor1_start.SetLineAppearance       ( kGray+2 , 1 , 4 ); 
  AnnotatedLine         Tor1_end ( "" , 16700-4.7e3-800   , plot::kMin , 16700-4.7e3-800   , plot::kMax ); Tor1_end.SetLineAppearance         ( kGray+2 , 1 , 4 ); 
  AnnotatedLine       Tor2_start ( "" , 16700-4.7e3       , plot::kMin , 16700-4.7e3       , plot::kMax ); Tor2_start.SetLineAppearance       ( kGray+2 , 1 , 4 ); 
  AnnotatedLine StopTgtSec_start ( "" , 16700             , plot::kMin , 16700             , plot::kMax ); StopTgtSec_start.SetLineAppearance ( kGray+2 , 1 , 4 ); 
  AnnotatedLine        EST_start ( "" , 21800             , plot::kMin , 21800             , plot::kMax ); EST_start.SetLineAppearance        ( kGray+2 , 1 , 4 ); 
  AnnotatedLine     DetSol_start ( "" , 28000             , plot::kMin , 28000             , plot::kMax ); DetSol_start.SetLineAppearance     ( kGray+2 , 1 , 4 ); 

  TFile* geomFile=TFile::Open("../Geometry-on-axis.root","READ");
  TH1* geomHist_noColl=NULL;
  if(geomFile){
	  geomHist_noColl=(TH1*)geomFile->Get("hGeometry");
  }

  geomFile=TFile::Open("~/comet/160306_geom_on_axis/WithCollimator_Geom.root","READ");
  TH1* geomHist_wColl=NULL;
  if(geomFile){
	  geomHist_wColl=(TH1*)geomFile->Get("hGeometry");
  }

  ///////////////////////   Beamline projections

//  TGaxis::SetMaxDigits();
  config.reset();
  config.y_axis_label_offset = 0.4;
  config.x_axis_label_offset = 0.8;
  config.y_axis_label_size = 0.07;
  config.x_axis_label_size = 0.07;
  config.y_axis_title_size = 0.065;
  config.x_axis_title_size = 0.07;
  config.x_axis_label    = "Distance Along Beamline (mm)";
  //config.y_axis_label    = "#  Height Above Beamline Axis (mm)";
  config.title = " ";
  //config.z_axis_label_centred= true;
  config.y_axis_range_high = 800;
  config.y_axis_range_low = -800;
  config.x_axis_range_low  = 17000;
  config.x_axis_range_high  = 33000;
  config.z_axis_range_high  = 0.05;
  config.z_axis_range_low  = 1e-6;
  config.shift_plot_y  = -0.05;
  //config.shift_palette  = -0.04;
  //config.canvas_ratio=0.8;
  //config.pad_margin_right= 0.14;
  //config.pad_margin_left= 0.10;
  config.canvas_width=3000;
  config.canvas_height=625;
  config.stats_force_off  = true;
  config.force_draw_option  = "colz";
  //config.marker_color=kWhite;
  config.grid_x=true; config.grid_y=true;
  config.log_z=true;

//  config.AddLine(Tor1_start);
//  config.AddLine(Tor1_end);
//  config.AddLine(Tor2_start);
//  config.AddLine(StopTgtSec_start);
//  config.AddLine(EST_start);
//  config.AddLine(DetSol_start);

  TString top_dir="~/comet/1602w06_EST_Dipole/";

  config.normalise=300000.;
  for(int i=0; i <3; ++i){
          file=FixPlot(top_dir+Form("signal_height-dipole_0.%d0.root",i),"TH1", config);
          if(geomHist_noColl) Overlay(geomHist_noColl,"same",config);
          SavePlot(ext,NULL,Form("Tidied_signal_height-dipole_%d0",i));
  }

  return;

  ///////////////////////   Mean height
  config.reset();
  config.y_axis_label_offset = 0.8;
  config.x_axis_label_offset = 0.8;
  config.y_axis_label_size = 0.04;
  config.x_axis_label_size = 0.04;
  config.y_axis_title_size = 0.05;
  config.x_axis_title_size = 0.05;
  config.x_axis_label    = "Distance Along Beamline (mm)";
  config.y_axis_label    = "  Height Above Beamline Axis (mm)";
  config.title = " ";
  //config.z_axis_label_centred= true;
  config.y_axis_range_high = 800;
  config.y_axis_range_low = -800;
  config.x_axis_range_low  = 17000;
  config.x_axis_range_high  = 33000;
  //config.z_axis_range_high  = 5e-4;
  //config.z_axis_range_low  = 5e-6;
  config.shift_plot_y  = -0.05;
  //config.shift_palette  = -0.04;
  config.canvas_ratio=1.3;
  //config.pad_margin_right= 0.14;
  //config.pad_margin_left= 0.10;
  config.stats_force_off  = true;
  config.line_width=3;
  config.legend_header="Dipole Field Strength (T)";
  config.legend_x1=0.14; config.legend_y1=0.27;
  config.legend_x2=0.38; config.legend_y2=0.56;
  config.legend_margin=0.5;
  config.legend_text_size=0.03;
  //config.marker_color=kWhite;
  config.grid_x=true; config.grid_y=true;
  config.log_z=true;

//  file=FixPlot(top_dir+"noShift_plots/NoShift-Height.root","THStack", config);
//  if(geomHist_noColl) Overlay(geomHist_noColl,"same",config);
//  SavePlot(ext);

  ///////////////////////   Survival vs. beam
  config.reset();
  config.y_axis_label_offset = 0.8;
  config.x_axis_label_offset = 0.8;
  config.y_axis_label_size = 0.04;
  config.x_axis_label_size = 0.04;
  config.y_axis_title_size = 0.05;
  config.x_axis_title_size = 0.05;
  config.x_axis_label    = "Distance Along Beamline (mm)";
  //config.y_axis_label    = "  Height Above Beamline Axis (mm)";
  //config.title = " ";
  //config.z_axis_label_centred= true;
  //config.y_axis_range_high = 800;
  //config.y_axis_range_low = -800;
  config.x_axis_range_low  = 17000;
  config.x_axis_range_high  = 33000;
  //config.z_axis_range_high  = 5e-4;
  //config.z_axis_range_low  = 5e-6;
  //config.shift_plot_y  = -0.05;
  //config.shift_palette  = -0.04;
  config.canvas_ratio=1.3;
  //config.pad_margin_right= 0.14;
  //config.pad_margin_left= 0.10;
  config.stats_force_off  = true;
  config.line_width=3;
  config.legend_header="Dipole Field Strength (T)";
  config.legend_x1=0.54; config.legend_y1=0.57;
  config.legend_x2=0.78; config.legend_y2=0.86;
  config.legend_margin=0.5;
  config.legend_text_size=0.03;
  //config.marker_color=kWhite;
  config.grid_x=true; config.grid_y=true;

  config.AddLine(EST_start);
  config.AddLine(DetSol_start);

//  file=FixPlot(top_dir+"noShift_plots/NoShift-Flux.root","THStack", config);
//  if(geomHist_noColl) Overlay(geomHist_noColl,"same",config);
//  SavePlot(ext);


  ///////////////////////   Acceptance vs. dipole
  config.reset();
  config.y_axis_label_offset = 0.8;
  config.x_axis_label_offset = 0.8;
  config.y_axis_label_size = 0.04;
  config.x_axis_label_size = 0.04;
  config.y_axis_title_size = 0.05;
  config.x_axis_title_size = 0.05;
  config.x_axis_label    = "Distance Along Beamline (mm)";
  config.y_axis_label    = "Geometric Acceptance";
  //config.title = " ";
  config.canvas_ratio=1.01;
  //config.pad_margin_right= 0.14;
  //config.pad_margin_left= 0.10;
  config.stats_force_off  = true;
  config.line_width=3;
  config.grid_x=true; config.grid_y=true;

  file=FixPlot(top_dir+"noShift_plots/noShift-Acceptance_adjacent.root","TMultiGraph", config);
  SavePlot(ext);

  file=FixPlot(top_dir+"noShift_plots/noShift-Acceptance-withFitt.root","TMultiGraph", config);
  SavePlot(ext);

}
