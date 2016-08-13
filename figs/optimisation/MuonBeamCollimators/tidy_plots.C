void tidy_plots(const char* ext="png"){

  gROOT->ProcessLine(".L ~/thesis/figs/tidy_plot_tools.C+");
   gStyle->SetStripDecimals(false);
  gStyle->SetPalette(55);
  gStyle->SetPaintTextFormat("0.3e");
//  gStyle->SetPadRightMargin(0.5);
  gStyle->SetTextColor(kWhite);

  TString top_dir="~/comet/1601w03_PhII_MuBeamOptimisation/";
  const TString dir_collim="~/comet/";
  PlotConfig config;

  AnnotatedLine       Tor1_start ( "" , 16700-4.7e3*2-800 , plot::kMin , 16700-4.7e3*2-800 , plot::kMax ); Tor1_start.SetLineAppearance       ( kGray+2 , 1 , 4 ); 
  AnnotatedLine         Tor1_end ( "" , 16700-4.7e3-800   , plot::kMin , 16700-4.7e3-800   , plot::kMax ); Tor1_end.SetLineAppearance         ( kGray+2 , 1 , 4 ); 
  AnnotatedLine       Tor2_start ( "" , 16700-4.7e3       , plot::kMin , 16700-4.7e3       , plot::kMax ); Tor2_start.SetLineAppearance       ( kGray+2 , 1 , 4 ); 
  AnnotatedLine StopTgtSec_start ( "" , 16700             , plot::kMin , 16700             , plot::kMax ); StopTgtSec_start.SetLineAppearance ( kGray+2 , 1 , 4 ); 
  AnnotatedLine        EST_start ( "" , 21800             , plot::kMin , 21800             , plot::kMax ); EST_start.SetLineAppearance        ( kGray+2 , 1 , 4 ); 
  AnnotatedLine     DetSol_start ( "" , 28000             , plot::kMin , 28000             , plot::kMax ); DetSol_start.SetLineAppearance     ( kGray+2 , 1 , 4 ); 

  TFile* geomFile=TFile::Open("Geometry-on-axis.root","READ");
  TH1* geomHist_noColl=NULL;
  if(geomFile){
	  geomHist_noColl=(TH1*)geomFile->Get("hGeometry");
  }

  geomFile=TFile::Open("~/comet/160306_geom_on_axis/WithCollimator_Geom.root","READ");
  TH1* geomHist_wColl=NULL;
  if(geomFile){
	  geomHist_wColl=(TH1*)geomFile->Get("hGeometry");
  }


//  ///////////////////////   Beam dispersion, no collimators
//  TGaxis::SetMaxDigits(3);
//  config.reset();
//  config.y_axis_label_offset = 1.40;
//  config.x_axis_label    = "Vertical Distance to Beam Axis (mm)";
//  config.y_axis_label    = "Momentum (MeV/c)";
//  config.z_axis_label    = "Muon flux per bin per POT";
//  config.z_axis_label_centred= true;
//  config.y_axis_decimal  = true;
//  config.x_axis_decimal  = true;
//  config.x_axis_range_high = 200;
//  config.z_axis_range_high = 9.5e-6;
//  config.x_axis_range_low  = -200;
//  config.y_axis_range_high = 300;
//  config.y_axis_range_low  = 0;
//  //config.shift_plot_x  = -0.02;
//  config.shift_palette  = -0.04;
//  config.canvas_ratio=0.8;
//  config.pad_margin_right= 0.14;
//  config.pad_margin_left= 0.10;
//  config.stats_force_off  = true;
//  config.force_draw_option  = "colz";
//  //config.marker_color=kWhite;
//  config.grid_x=true; config.grid_y=true;
//
//  TString top_dir="~/comet/1601w03_PhII_MuBeamOptimisation/";
//
//  config.title="Muon Dispersion at Entrance to Bent Solenoid";
//  file=FixPlot(top_dir+"T1_1.00-T2_0.50-MuonsAtTor1Entrance.root","TH1", config);
//  SavePlot(ext,NULL,"Tidied_Muon_dispersion_at_entrance");
//
//  config.title="Muon Dispersion at Exit of Bent Solenoid";
//  file=FixPlot(top_dir+"T1_1.00-T2_0.50-MuonsAtTor2Exit.root","TH1", config);
//  SavePlot(ext,NULL,"Tidied_Muon_dispersion_at_exit");

  ///////////////////////   Beamline projections

  TGaxis::SetMaxDigits();
  config.reset();
  config.y_axis_label_offset = 0.5;
  config.x_axis_label    = "Distance Along Beamline (mm)";
  config.y_axis_label    = "Height Above Beamline Axis (mm)";
  config.z_axis_label_centred= true;
  config.y_axis_range_high = 800;
  config.y_axis_range_low = -800;
  config.x_axis_range_low  = 5000;
  config.x_axis_range_high  = 24000;
  config.z_axis_range_high  = 5e-4;
  //config.z_axis_range_low  = 5e-6;
  //config.shift_plot_x  = -0.02;
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

  config.AddLine(Tor1_start);
  config.AddLine(Tor1_end);
  config.AddLine(Tor2_start);
  config.AddLine(StopTgtSec_start);
  config.AddLine(EST_start);
  config.AddLine(DetSol_start);

  TString top_dir="~/comet/160220_MuonBeamFull/";

  const char* beamline_plots[]={
          "All_muons_wGeom.root",
          "HighP_muons_wGeom.root",
          "Where_to_collimate_wGeom.root",
          "Stopped_muons_wGeom.root"};
  bool logZ[]={true,true,false,true};
//  config.normalise=9900000.;
//  for(int i=0; i <4; ++i){
//          config.log_z=logZ[i];
//          file=FixPlot(top_dir+beamline_plots[i],"TH1", config);
//          if(geomHist_noColl) Overlay(geomHist_noColl,"same",config);
//          SavePlot(ext);
//  }

  
  top_dir="~/comet/160200_MassProduction_MuBeam/160303_analysis-files_116/plots/";

  const char* beamline_plots_coll[]={
          "AllMuons_WGeom",
          "HighPMuons_WGeom",
          "StoppedMuons_WGeom"};
  config.log_z=true;
  config.normalise=8518900.;
  TString pre_name="Tidied_WColl_";
  for(int i=0; i <3; ++i){
          file=FixPlot(top_dir+beamline_plots_coll[i]+".root","TH1", config);
          if(geomHist_wColl) Overlay(geomHist_wColl,"same",config);
          SavePlot(ext,NULL,pre_name+beamline_plots_coll[i]);
  }

}
