void tidy_plots(const char* ext="png"){

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

  TString top_dir="~/comet/1603w12_Phase-II_estimates/pions_in_beam/";
  //TString top_dir=gSystem->ExpandPathName("$PWD/");
  //====================================================
  //config.y_axis_label_offset = 1.3;
  config.y_axis_decimal  = true;
//  config.x_axis_range_high  = 110;
//  config.y_axis_range_high  = 110;
//  config.y_axis_label  = "Observed Momentum (MeV/c)";
  config.y_axis_label  = "Pion Stops per {x_bin_widths} mm";
//  config.log_y=false; config.log_z=true;
  config.grid_y=config.grid_x=true;
  config.stats_force_off=true;
  config.canvas_ratio=1.05;
  config.title=" ";
  config.y_axis_label_size=0.05;
  config.x_axis_label_size=0.05;
  config.y_axis_title_size=0.05;
  config.x_axis_title_size=0.05;
  config.shift_plot_x=0.03;
  config.rebin=2;

  const char* axes[]={"X","Y","Z"};
  for(int i=0; i<3; ++i){
		  new TCanvas;
     if(i==2){
        config.y_axis_label_offset=1.1;
        config.x_axis_range_low=950;
        config.x_axis_range_high=1900;
        config.x_axis_divisions=505;
     }
     file=DrawFixPlot(top_dir+Form("stopped_hPosition%s_-211.root",axes[i]),Form("hPosition%s_-211",axes[i]),"TH1", config);
     SavePlot(ext,NULL,Form("Tidied_StoppedPi-%s",axes[i]));
  }:

  //====================================================
  config.reset();
  top_dir="~/comet/1603w12_Phase-II_estimates/";
  config.y_axis_decimal  = true;
  config.y_axis_label_size=0.05;
  config.x_axis_label_size=0.05;
  config.y_axis_title_size=0.05;
  config.x_axis_title_size=0.05;
  config.grid_y=config.grid_x=true;
  config.canvas_ratio=1.4;
  config.canvas_grow=2;
  config.log_y=true;
  config.shift_plot_y=-0.05;
  config.title=" ";
  config.x_axis_label="Momentum (MeV/c)";
  config.y_axis_label="Flux per POT";
  config.line_width=2;

  config.x_axis_range_low=5; config.x_axis_range_high=120;
  config.y_axis_range_low=5e-10; config.y_axis_range_high=2e-3;
  config.legend_x1=0.11; config.legend_x2=0.89;
  config.legend_y1=0.87; config.legend_y2=0.99;
  config.legend_columns=2;
  config.legend_margin=0.45;

  file=FixPlot(top_dir+"160906_MuVsPiMomentum.root","THStack", config);
  SavePlot(ext,NULL,"Tidied_MuVsPiMomentum");

  //config.reset();
  //config.legend_columns=5;
  //config.legend_margin=0.5;
  //config.legend_text_size= 0.024;
  //config.legend_x1  = 0.45; config.legend_y1  = 0.75;
  //config.legend_x2  = 0.88; config.legend_y2  = 0.88;
}
