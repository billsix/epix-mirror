#!/bin/bash
#
# Apr 22, 2003 --  Create "sample.tex" from existing .xp files
# Sep 04, 2004 --  Updates for Version 1.0.0
#
OUTFILE="sample.tex"

my_decode="tr '[a-m][n-z]' '[n-z][a-m]'" # Mac OS X has no 'rot'
SPAMMY="<$(echo nujnat|$my_decode)@$(echo zngupf|$my_decode)."
SPAMMY="${SPAMMY}$(echo ubylpebff|$my_decode).$(echo rqh|$my_decode)>"

function get_file {
    cat $1 | grep -v offset | grep -v "\-\*\-" | grep -v "\-\-" >> $OUTFILE
}

cat <<HEADER > $OUTFILE
%% Sample LaTeX file for ePiX                September, 2004
%%
%% Andrew D. Hwang  $SPAMMY
%% Department of Mathematics and Computer Science
%% College of the Holy Cross
%% Worcester, MA 01610-2395, USA
%%
HEADER
cat <<"SEGMENT" >> $OUTFILE
\documentclass[11pt]{article}
\usepackage[leqno]{amsmath}
\usepackage{latexsym,pstcol,epic,eepic,rotating}

\newcommand{\ePiX}{\texttt{ePiX}}
\newcommand{\C}{\texttt{C}}
\newcommand{\R}{\mathbf{R}}

\DeclareMathOperator{\re}{Re}
\DeclareMathOperator{\im}{Im}

\setlength{\textheight}{9.5in}
\setlength{\textwidth}{6in}
\setlength{\oddsidemargin}{0.25in}
\setlength{\topmargin}{0in}

\title{\ePiX\ Sample Document}
\author{Version 1.0}
\date{September, 2004}

\begin{document}

\maketitle

\section{Overview}

\ePiX\ is a powerful, flexible, lightweight utility for creating
mathematically accurate \LaTeX\ plots and figures from simple,
mnemonic commands. A detailed user's manual is available in several
formats from
\begin{verbatim}
http://math.holycross.edu/~ahwang/current/ePiX.html
\end{verbatim}
This sample document demonstrates a few of \ePiX's capabilities, with
side-by-side comparisons of input files\footnote{The sample files
distributed with \ePiX\ have \texttt{offset} lines to place the figures
next to their source code.} and corresponding output. \ePiX\ has several 
distinguishing features:
\begin{itemize}

\item Scenes are described in mathematically natural Cartesian
coordinates, making \ePiX\ effectively a vector format. Well-designed
figures are of camera quality over a range of sizes and aspect ratios.
There are almost no default choices; aspect ratio, color, viewpoint
(for 3-D figures) and line width are wholly controllable.

\item High-quality typography is easily incorporated.

\item All the power of \texttt{C++} is available in using and extending 
the program's capabilities.

\item The license, the GNU GPL, is similar to the terms on theorems:
You may run \ePiX\ for any purpose, examine the code, study how the
program works, make improvements, and distribute your improvements so
long as you do not restrict the rights of others to do the same.

\end{itemize}

\section{Examples}

\ePiX\ provides traditional plotting capabilities, such as Cartesian
and polar plots, data plotting from a file, and parametric curves and
surfaces (without automatic hidden object removal). As mentioned, the 
implementation allows \ePiX\ to be regarded as a programming language;
any numerical algorithm written in \texttt{C}~or \texttt{C++} can be 
used in an \ePiX\ figure. The use of programming constructs can make a
figure flexible (so that the appearance can be precisely but dramatically
altered by making a few small changes to the input file) and easier to
maintain.

The figures below were chosen to emphasize results that are relatively
difficult to achieve with existing Free (and some commercial) plotting
software.  Simple line figures containing polygons, ellipses, and
splines are almost self-explanatory, so no examples are given. 

In the files below, several short statements are often put on a single
line to make the file fit on a page; this is not good programming
practice, and should be avoided in real files. Several sample files
could be shortened by hard-wiring constants.

\clearpage

\begin{figure}[hbt]
\begin{center}
\input{parabola.eepic}
\vspace*{-2in}
\begin{verbatim}
SEGMENT

get_file parabola.xp

cat <<"SEGMENT" >> $OUTFILE
\end{verbatim}
\caption{A basic plot.}
\label{fig:parabola}
\end{center}
\end{figure}

\clearpage

\begin{figure}[hbt]
\begin{center}
\input{cropplot.eepic}
\vspace*{-200pt}
\begin{small}
\begin{verbatim}
SEGMENT

get_file cropplot.xp

cat <<"SEGMENT" >> $OUTFILE
\end{verbatim}
\end{small}
\caption{``Cropping'' removes elements outside the bounding box.}
\label{fig:cropplot}
\end{center}
\end{figure}

\clearpage

\noindent There are angular ``modes'' for polar and spherical
coordinates. \ePiX\ defines its own trig functions, which are
sensitive to the current mode.

\begin{figure}[hbt]
\begin{center}
\input{polar.eepic}
\vspace*{-150pt}
\begin{verbatim}
SEGMENT

get_file polar.xp

cat <<"SEGMENT" >> $OUTFILE
\end{verbatim}
\caption{The polar graph $r=2\cos 3\theta$ for $0\leq\theta\leq\pi$.}
\label{fig:polar}
\end{center}
\end{figure}

\clearpage

\noindent Regions between graphs can be shaded; gray density ranges
from~0 (white) to~1 (black).

\begin{figure}[hbt]
\begin{center}
\input{shadeplot.eepic}
\vspace*{-170pt}
\begin{footnotesize}
\begin{verbatim}
SEGMENT

get_file shadeplot.xp

cat <<"SEGMENT" >> $OUTFILE
\end{verbatim}
\end{footnotesize}
\caption{The first fundamental theorem of calculus: $F'(x)=f(x)+o(1)$.}
\label{fig:shadeplot}
\end{center}
\end{figure}

\clearpage

\noindent Control structures---loops and decision statements---can be
used to create input files whose logical structure matches the
mathematical structure of the figure.

\begin{figure}[hbt]
\begin{center}
\input{newton.eepic}
\vspace*{-2.5in}
\begin{footnotesize}
\begin{verbatim}
SEGMENT

get_file newton.xp

cat <<"SEGMENT" >> $OUTFILE
\end{verbatim}
\end{footnotesize}
\caption{Newton's method for root approximation}
\label{fig:newton}
\end{center}
\end{figure}

\clearpage

\noindent\ePiX\ can graph derivatives and definite integrals:

\begin{figure}[hbt]
\begin{center}
\input{calculus.eepic}
\vspace*{-120pt}
\begin{small}
\begin{verbatim}
SEGMENT

get_file calculus.xp

cat <<"SEGMENT" >> $OUTFILE
\end{verbatim}
\end{small}
\caption{$y=x\sin x$ (black), its derivative (green) and integral from~$0$
(blue).}
\label{fig:calculus}
\end{center}
\end{figure}

\clearpage

\noindent Solutions of ODEs are computed with Euler's method. Vector 
fields may be plotted at constant (shown) or true length.

\begin{figure}[ht]
\begin{center}
\input{slopefield.eepic}
\vspace*{-180pt}
%%\begin{footnotesize}
\begin{verbatim}
SEGMENT

get_file slopefield.xp

cat <<"SEGMENT" >> $OUTFILE
\end{verbatim}
%%\end{footnotesize}
\caption{A slope field and six solutions of an ODE.}
\label{fig:slopefield}
\end{center}
\end{figure}

\clearpage

\noindent The ``denominator'' function~$f$, defined by
\begin{equation*}
f(x)=\begin{cases}
\frac{1}{q} & \text{if $x=\frac{p}{q}$ in lowest terms} \\
0 & \text{if $x$~is irrational}
\end{cases}
\end{equation*}
can be plotted with a nested \texttt{for} loop.

\begin{figure}[hbt]
\begin{center}
\input{denom.eepic}
\vfil

\begin{verbatim}
SEGMENT

get_file denom.xp

cat <<"SEGMENT" >> $OUTFILE
\end{verbatim}
\caption{A "pathological" function in real analysis.}
\label{fig:denom}
\end{center}
\end{figure}

\clearpage

\noindent Finite sums are defined by an algorithm and are therefore
easy to plot. The function being scaled and summed is~cb, the
``Charlie Brown'' function (blue). Nowhere differentiability is
essentially obvious from the picture, because the graph is
self-similar (red) and not a line.

\begin{figure}[hbt]
\begin{center}
\input{weierstrass.eepic}
\vspace*{-1.375in}
\begin{small}
\begin{verbatim}
SEGMENT

get_file weierstrass.xp

cat <<"SEGMENT" >> $OUTFILE
\end{verbatim}
\end{small}
\caption{A Weierstrass nowhere-differentiable function.}
\label{fig:weierstrass}
\end{center}
\end{figure}

\clearpage

\ePiX\ can approximate the extreme values of a function on an
interval, which is useful for drawing inscribed or circumscribed
rectangles in a graph. The sine function is symbolized by~\texttt{f}
throughout the body (except the label, of course), so re-drawing the
figure with a different integrand only requires changing the
definition of~\texttt{f} and re-sizing the \verb+bounding_box+.  The
number of rectangles is similarly ``parametrized''.

\begin{figure}[hbt]
\begin{center}
\input{uppersum.eepic}
\vspace*{-1in}
\begin{small}
\begin{verbatim}
SEGMENT

get_file uppersum.xp

cat <<"SEGMENT" >> $OUTFILE
\end{verbatim}
\end{small}
\caption{Upper and lower sums for $\displaystyle\int_0^3\sin x\,dx$.}
\label{fig:uppersum}
\end{center}
\end{figure}

\clearpage

A loop index may be used to control an entire figure, generating a
sequence of snapshots of a time-varying picture, such as a rolling
wheel, or the flow of an ODE. The \ePiX\ project page contains links
to animations that can be played in any web browser.

\begin{small}
\begin{verbatim}
SEGMENT

get_file wheel.xp

cat <<"SEGMENT" >> $OUTFILE
\end{verbatim}
\end{small}

\vspace*{-100pt}
\begin{figure}[hb]
\begin{center}
\input{wheel.eepic}
\caption{Snapshots of cycloids.}
\label{fig:wheel}
\end{center}
\end{figure}

\clearpage

Figure~\ref{fig:sphere} (by Jacques L'helgoual) demonstrates some
spherical geometry capabilities: Space curves can be projected to the
unit sphere radially, and plane curves can be projected
stereographically from the north or south pole, with or without hidden
line removal.

\begin{figure}[hbt]
\begin{center}
\input{sphere.eepic}
\vspace*{-150pt}
\begin{scriptsize}
\begin{verbatim}
SEGMENT

get_file sphere.xp

cat <<"SEGMENT" >> $OUTFILE
\end{verbatim}
\end{scriptsize}
\caption{Loxodromes on the unit sphere.}
\label{fig:sphere}
\end{center}
\end{figure}

\clearpage

\noindent \texttt{PSTricks} can be incorporated in \ePiX\ files to 
achieve colored filling and other effects. The \texttt{psset} command
puts its argument into the output file.

\begin{figure}[hbt]
\begin{center}
\input{geomsum.eepic}
\vspace*{-192pt}
%%\begin{small}
\begin{verbatim}
SEGMENT

get_file geomsum.xp

cat <<"SEGMENT" >> $OUTFILE
\end{verbatim}
%\end{small}
\caption{A geometric series with ratio $1/2$.}
\label{fig:geomsum}
\end{center}
\end{figure}

\clearpage

\noindent \ePiX\ computes intersections of geometric objects such as lines,
circles, and planes.
%% In this file, newlines have been omitted to make the file fit on one 
%% page; generally it is a good idea to have only one statement per line.

\begin{figure}[hbt]
\begin{center}
\input{pascal.eepic}
\vspace*{-2.5in}
\begin{footnotesize}
\begin{verbatim}
SEGMENT

get_file pascal.xp

cat <<"SEGMENT" >> $OUTFILE
\end{verbatim}
\end{footnotesize}
\caption{Pascal's theorem on sides of a hexagon.}
\label{fig:pascal}
\end{center}
\end{figure}

\clearpage

\noindent Paths may be built in pieces and manipulated as a single object.

\begin{figure}[hbt]
\begin{center}
\input{contour.eepic}
\vspace*{-2.25in}
\begin{footnotesize}
\begin{verbatim}
SEGMENT

get_file contour.xp

cat <<"SEGMENT" >> $OUTFILE
\end{verbatim}
\end{footnotesize}
\caption{A keyhole contour for a branch cut integral.}
\label{fig:contour}
\end{center}
\end{figure}

\clearpage

\noindent \ePiX\ provides ``clipping'', a 3-dimensional analogus of 
cropping; figure elements that lie outside a coordinate box are removed.

\begin{figure}[hbt]
\begin{center}
\input{pole.eepic}
\vspace*{-2.25in}
\begin{verbatim}
SEGMENT

get_file pole.xp

cat <<"SEGMENT" >> $OUTFILE
\end{verbatim}
\caption{The real part of the complex reciprocal.}
\label{fig:pole}
\end{center}
\end{figure}

\clearpage

\begin{figure}[hbt]
\begin{center}
\input{sqrt.eepic}
\vspace*{-2.25in}
\begin{footnotesize}
\begin{verbatim}
SEGMENT

get_file sqrt.xp

cat <<"SEGMENT" >> $OUTFILE
\end{verbatim}
\end{footnotesize}
\caption{Two sheets of the Riemann surface of~$\sqrt{z}$.}
\label{fig:sqrt}
\end{center}
\end{figure}

\clearpage

\subsection*{Gallery}

\noindent The input files for the figures below are in the
\texttt{samples} directory of the source tree.

\begin{center}
\input{lissajous.eepic}\hspace*{1in}
\input{koch.eepic}
\vspace*{0.5in}

\input{oscillator.eepic}\hspace*{1in}
\input{torus.eepic}
\vfil

\input{levelset.eepic}\hspace*{1in}
\input{clipping.eepic}
\end{center}

\end{document}
SEGMENT

exit 0
