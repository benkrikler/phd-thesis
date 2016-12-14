\ProvidesClass{thesis}
%% For normal draft builds
\LoadClass[nobind,draft,rmtitles,oneside]{hepthesis}
%\LoadClass[hyperpdf,nobind,draft,twoside]{hepthesis}
%% For short draft builds (breaks citations by necessity)
%\LoadClass[hyperpdf,nobind,draft,hidefrontback]{hepthesis}
%% For Cambridge soft-bound version
%\LoadClass[hyperpdf,bindnopdf]{hepthesis}
%% For Cambridge hard-bound version (must be one-sided)
%\LoadClass[hyperpdf,oneside]{hepthesis}

%% -------------------------------------
%% Standard packages
%% -------------------------------------
\RequirePackage{cite}
%\RequirePackage{morefloats}
\RequirePackage{mathrsfs} % script font
\RequirePackage{afterpage}
%\RequirePackage{gillius}
%% Using Babel allows other languages to be used and mixed-in easily
\RequirePackage[english]{babel}
\selectlanguage{english}

%% Quotes in different languages are different.
%% Using \enquote{bla} automatically uses the correct quotes
%% according to the Babel language that is set
\RequirePackage[babel]{csquotes}

%% Subfigures - note subfig is incompatible with ccaption
\RequirePackage{subfig}

%% TikZ is a marvellous drawing package
\RequirePackage{tikz}

%% BEK: Switch off problematic full-stops after sections, figure, and table 
%% numbers when using an appendix
\KOMAoptions{numbers=noenddot}

%% BEK: Remove extra vspace inserted when a table float is created
%% Table environment
\renewenvironment{table}[1][tbp]{%
  \@oldtable[#1]%
  \begin{center}%
}{%
  \end{center}%
  \@oldendtable%
  \ignorespacesafterend%
}
%% Makes a thesis title page
\DeclareRobustCommand{\titlepage}[2][]{%
  %\@oldtitlepage%
  \thispagestyle{empty}%
  \begingroup%
  \ifx\@sftitles\@empty\else\sf\fi%
  \begin{center}%
    \vspace*{\frontmattertitleskip}%
    \begin{doublespace}%
      {\Huge\textbf{\thetitle}}\\%
    \end{doublespace}%
    \vspace*{1cm}%
    {\Large{{\theauthor} \\ {#1}}}\\%
    {#2}%
  \end{center}%
  \endgroup%
  %\@oldendtitlepage%
  %\cleardoublepage%
}

%% An environment for the rubbish at the front of a typical thesis
\specialcomment{frontmatter}{%
  \setcounter{tocdepth}{2}% show down to subsections in contents
  \setcounter{secnumdepth}{2}% number down to subsections
  \newcounter{iterlist}%
  \@oldfrontmatter%
  \pagenumbering{arabic}%
  \adjustwidth{\@frontmatterextramargin}{\@frontmatterextramargin}%
  \begin{\@frontmatterspacing}%
}{%
  \end{\@frontmatterspacing}%
  \endadjustwidth%
  \ignorespacesafterend%
  %\newcounter{grabPageNumber}%
  %%\setcounter{\value{page}}%
}

\specialcomment{mainmatter}{%
  %\@oldmainmatter%
  \@mainmattertrue%
  %\pagenumbering{arabic}%
  %\setcounter{page}{\value{grabPageNumber}}%
  %\pagenumbering{arabic}%
  \adjustwidth{\@mainmatterextramargin}{\@mainmatterextramargin}%
  \begin{\@mainmatterspacing}%
}{%
  \end{\@mainmatterspacing}%
  \endadjustwidth%
  \ignorespacesafterend%
}


