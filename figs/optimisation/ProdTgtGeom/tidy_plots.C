void tidy_plots(){

  gROOT->ProcessLine(".L ~/thesis/figs/tidy_plot_tools.C+");

  const TString top_dir="~/comet/1512w51_PhII_ProdTgtSec/";
  PlotConfig config;

  //====================================================
  // MOMENTUM SPECTRA
  config.reset();
  config.y_axis_label_offset = 1.6;
  config.y_axis_label    = "Differential Yield ( #times 10^{-3} per MeV/c)";
  config.y_axis_decimal  = true;
  config.legend_header   = "Target Half-Length (cm)";
  config.legend_text_size= 0.022;
  config.legend_x1  = 0.6 ; config.legend_y1  = 0.6;
  config.legend_x2  = 0.85; config.legend_y2  = 0.85;
  config.shift_plot_x  = 0.05;

  // Length, muons
  file=FixPlot(top_dir+"length_optimisation/mu-minus_momentum.root","THStack", config);
  SavePlot("Length_mu-minus_momentum","pdf");
  // Length, pions
  file=FixPlot(top_dir+"length_optimisation/pi-minus_momentum.root","THStack", config);
  SavePlot("Length_pi-minus_momentum","pdf");

  // Radius, muons
  file=FixPlot(top_dir+"radius_optimisation/mu-minus_momentum.root","THStack", config);
  SavePlot("Radius_mu-minus_momentum","pdf");
  // Radius, pions
  file=FixPlot(top_dir+"radius_optimisation/pi-minus_momentum.root","THStack", config);
  SavePlot("Radius_pi-minus_momentum","pdf");

  //====================================================
  // INTEGRATED SPECTRA
  config.reset();
  config.y_axis_label    = "Particle Yield per Proton Below a Given Momentum (#kern[2.0]{ }#times 10^{-3}#kern[1.0]{)}";
  //config.y_axis_label    = "\\text{Particle Yield per Proton Below a Given Momentum (}\\times 10^{-3}~\\text{)}";
  config.y_axis_decimal  = true;
  config.y_axis_range_low  = 0;
  config.legend_header   = "Momentum (MeV/c)";
  //config.legend_text_size= 0.025;
  config.legend_x1  = 0.11; config.legend_y1  = 0.8;
  config.legend_x2  = 0.89; config.legend_y2  = 0.89;
  config.legend_columns = 5;

  AnnotatedLine cdr_length("Length in CDR '09", 8, plot::kMin, 8, plot::kMax);
  AnnotatedLine cdr_radius("Radius in CDR '09", 4, plot::kMin, 4, plot::kMax);
  AnnotatedLine optimal_length("Optimised Length", 16, plot::kMin, 16, plot::kMax);
  AnnotatedLine optimal_radius("Optimised Radius", 10, plot::kMin, 10, plot::kMax);
  cdr_length.SetTextAlignHor(AnnotatedLine::kBegin,0.1);
  cdr_radius.SetTextAlignHor(AnnotatedLine::kEnd,-0.2);
  optimal_length.SetTextAlignHor(AnnotatedLine::kBegin,0.1);
  optimal_radius.SetTextAlignHor(AnnotatedLine::kBegin,0.1);

  config.x_axis_label    = "Target Half Length (cm)";
  config.AddLine(cdr_length);
  config.AddLine(optimal_length);
  // Length, muon
  config.y_axis_range_high = 12.5;
  file=FixPlot(top_dir+"length_optimisation/mu-minus_integral_toZero.root","TMultiGraph", config);
  SavePlot("Length_mu-minus_integral_toZero","pdf");
  // Length, pion
  config.y_axis_range_high = 8;
  file=FixPlot(top_dir+"length_optimisation/pi-minus_integral_toZero.root","TMultiGraph", config);
  SavePlot("Length_pi-minus_integral_toZero","pdf");

  config.x_axis_label    = "Target Radius (mm)";
  config.ClearLines();
  config.AddLine(cdr_radius);
  config.AddLine(optimal_radius);
  // Radius, muon
  config.y_axis_range_high = 18;
  file=FixPlot(top_dir+"radius_optimisation/mu-minus_integral_toZero.root","TMultiGraph", config);
  SavePlot("Radius_mu-minus_integral_toZero","pdf");
  // Radius, pion
  config.y_axis_range_high = 11;
  file=FixPlot(top_dir+"radius_optimisation/pi-minus_integral_toZero.root","TMultiGraph", config);
  SavePlot("Radius_pi-minus_integral_toZero","pdf");

  //====================================================
  // SPECTRAL SHAPE CHANGES
  config.reset();
  config.y_axis_label    = "Fraction of Distribution Below a Given Momentum";
  config.y_axis_label_offset = 1.3;
  config.y_axis_decimal  = true;
  config.legend_header   = "Momentum (MeV/c)";
  config.legend_text_size= 0.025;
  config.legend_x1  = 0.11; config.legend_y1  = 0.8;
  config.legend_x2  = 0.89; config.legend_y2  = 0.89;
  config.y_axis_range_high = 1.18;
  config.legend_columns = 5;

  config.x_axis_label    = "Target Half Length (cm)";
  // Length, muons
  file=FixPlot(top_dir+"length_optimisation/mu-minus_integral_ratios.root","TMultiGraph", config);
  SavePlot("Length_mu-minus_integral_ratios","pdf");
  // Length, pions
  file=FixPlot(top_dir+"length_optimisation/pi-minus_integral_ratios.root","TMultiGraph", config);
  SavePlot("Length_pi-minus_integral_ratios","pdf");

  config.x_axis_label    = "Target Radius (mm)";
  // Radius, muons
  file=FixPlot(top_dir+"radius_optimisation/mu-minus_integral_ratios.root","TMultiGraph", config);
  SavePlot("Radius_mu-minus_integral_ratios","pdf");
  // Radius, pions
  file=FixPlot(top_dir+"radius_optimisation/pi-minus_integral_ratios.root","TMultiGraph", config);
  SavePlot("Radius_pi-minus_integral_ratios","pdf");

}
