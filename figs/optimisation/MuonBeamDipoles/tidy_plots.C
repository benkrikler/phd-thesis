void tidy_plots(const char* ext="png"){

  gROOT->ProcessLine(".L ~/thesis/figs/tidy_plot_tools.C+");
  TGaxis::SetMaxDigits(3);
   gStyle->SetStripDecimals(false);
  gStyle->SetPalette(55);
  gStyle->SetPaintTextFormat("0.3e");
//  gStyle->SetPadRightMargin(0.5);
  gStyle->SetTextColor(kWhite);

  const TString top_dir="~/comet/1601w03_PhII_MuBeamOptimisation/dipole_optimisation_analysis/";
  PlotConfig config;

  ///////////////////////   Stopping Rate
  config.reset();
  config.y_axis_label_offset = 1.00;
  config.y_axis_label    = "Torus 2 Scale Factor ( #times 0.055 T)";
  config.x_axis_label    = "Torus 1 Scale Factor ( #times 0.055 T)";
  config.z_axis_label_centred= true;
  config.y_axis_decimal  = true;
  config.x_axis_decimal  = true;
  config.x_axis_range_high = 1.3;
  config.x_axis_range_low  = 0.7;
  config.y_axis_range_high = 1.3;
  config.y_axis_range_low  = 0;
  //config.shift_plot_x  = -0.03;
  config.shift_palette  = -0.04;
  config.canvas_ratio=1.1;
  config.pad_margin_right= 0.14;
  config.pad_margin_left= 0.08;
  config.stats_force_off  = true;
  config.force_draw_option  = "colzttext";
  config.marker_color=kWhite;

  config.z_axis_label    = "Muon Stops per POT";
  file=FixPlot(top_dir+"stopped_muons.root","", config);
  SavePlot(ext);

  config.z_axis_label    = "Pion Stops per POT";
  file=FixPlot(top_dir+"stopped_pions.root","", config);
  SavePlot(ext);
}
