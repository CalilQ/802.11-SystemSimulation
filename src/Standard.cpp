/*
* Copyright (c) 2002-2015 by Microwave and Wireless Systems Laboratory, by Andre Barreto and Calil Queiroz
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include <iostream>

#include "Standard.h"
#include "Packet.h"
#include "myexception.h"
#include "timestamp.h"

// Static member variables need to be defined outside the class
dot11_standard Standard::currentStd = dot11;
transmission_mode Standard::maxMCS = MCS;
double Standard::symbol_period = 4e-6;
channel_bandwidth Standard::maxBand = MHz;
channel_bandwidth Standard::band = MHz;
bool Standard::shortGI = false;

//Data rates
double Standard::rates_a[8]       =  {    6,    9,   12,   18,   24,   36,   48,   52};
double Standard::rates_n[2][8][2] = {{{ 6.5, 13.5},
									 {   13,   27},
									 { 19.5, 40.5},
									 {   26,   54},
									 {   39,   81},
									 {   52,  108},
									 { 58.5,121.5},
									 {   65,  135}},

									 {{ 7.2,   15},
									 { 14.4,   30},
									 { 21.7,   45},
									 { 28.9,   60},
									 { 43.3,   90},
									 { 57.8,  120},
									 {   65,  135},
									 { 72.2,  150}}};

double Standard::rates_ac_ah[2][10][4] = {{{  6.5, 13.5, 29.3, 58.5},
									       {   13,   27, 58.5,  117},
										   { 19.5, 40.5, 87.8,175.5},
									       {   26,   54,  117,  234},
										   {   39,   81,175.5,  351},
										   {   52,  108,  234,  468},
										   { 58.5,121.5,263.3,526.5},
										   {   65,  135,292.5,  585},
										   {   78,  162,  351,  702},
										   {    0,  180,  390,  780}},

										  {{  7.2,   15, 32.5,   65},
										   { 14.4,   30,   65,  130},
										   { 21.7,   45, 97.5,  195},
										   { 28.9,   60,  130,  260},
										   { 43.3,   90,  195,  390},
										   { 57.8,  120,  260,  520},
										   {   65,  135,292.5,  858},
										   { 72.2,  150,  325,  650},
										   { 86.7,  180,  390,  780},
										   {    0,  200,433.3,866.7}}};


// Bits per OFDM symbol
unsigned Standard::bits_per_symb_a[8]    	  =  {  24,  36,  48,  72,  96, 144, 192, 216};
unsigned Standard::bits_per_symb_n[8][2] 	  = {{  26,  54},
											     {  52, 108},
												 {  78, 162},
												 { 104, 216},
											     { 156, 324},
												 { 208, 432},
												 { 234, 486},
												 { 260, 540}};
unsigned Standard::bits_per_symb_ac_ah[10][4] = {{  26,  54, 117, 234},
												 {  52, 108, 234, 468},
												 {  78, 162, 351, 702},
												 { 104, 216, 468, 936},
												 { 156, 324, 702,1404},
												 { 208, 432, 936,1872},
												 { 234, 486,1053,2106},
												 { 260, 540,1170,2340},
												 { 312, 648,1404,2808},
												 {	 0, 720,1560,3120}};

// Standard error model polynomials
//802.11a
double Standard::min_thresh_a[8] = {-2.5103, 0.75061,  0.5000,  3.7609,
        5.5103, 9.2712 , 12.5206, 14.5321};
double Standard::max_thresh_a[8] = { 1.9897,  5.2506,  5.0000,  8.2609,
        10.5103, 14.7712, 18.5206, 20.0321};
double Standard::coeff_a[8][5] = {
        {  -2.2353000, -1.0721000,-0.1708900, 0.0243860, 0.0096656},
        {  -0.3624500, -0.2937100,-0.0011057,-0.0408500, 0.0038022},
        {  -0.4517200, -0.3560888, 0.0627930,-0.0651410, 0.0064799},
        {  -0.3082200, -0.2063400, 0.1547100,-0.0389730, 0.0018157},
        {   2.6965000, -1.9353000, 0.4736600,-0.0509360, 0.0016224},
        {  34.8692000,-13.9070000, 2.0328000,-0.1283100, 0.0028499},
        {  93.9622000,-26.7075000, 2.8106000,-0.1290100, 0.0021372},
        {-120.1972000, 26.3772000,-2.1564000, 0.0787190,-0.0011189}};
double Standard::coeff_high_a[8][2] = { {-2.3974,-1.1580}, { 2.8250,-1.4824},
        { 2.1138,-1.3738}, { 7.7079,-1.5347},
        { 9.2576,-1.3244}, {11.3789,-1.1004},
        {14.6479,-1.0454}, {20.0742,-1.2278} };

//802.11n
double Standard::min_thresh_n[8] = {-2.5103, 0.5000, 3.7609, 5.5103, 9.2712,
		12.5206,14.5321,11.6188};
double Standard::max_thresh_n[8] = { 1.9897,  5.0000,  8.2609, 10.5103, 14.7712,
		18.5206, 20.0321, 21.5000};
double Standard::coeff_n[8][5] = {
		{  -2.2353000, -1.0721000,-0.1708900, 0.0243860, 0.0096656},
		{  -0.4517200, -0.3560888, 0.0627930,-0.0651410, 0.0064799},
		{  -0.3082200, -0.2063400, 0.1547100,-0.0389730, 0.0018157},
		{   2.6965000, -1.9353000, 0.4736600,-0.0509360, 0.0016224},
		{  34.8692000,-13.9070000, 2.0328000,-0.1283100, 0.0028499},
		{  93.9622000,-26.7075000, 2.8106000,-0.1290100, 0.0021372},
		{-120.1972000, 26.3772000,-2.1564000, 0.0787190,-0.0011189},
		{ -42.9748720, 11.0385340,-1.0249930, 0.0411790,-0.0006080}};
double Standard::coeff_high_n[8][2] = {
		{-2.3974,-1.1580}, { 2.1138,-1.3738}, { 7.7079,-1.5347},
		{ 9.2576,-1.3244}, {11.3789,-1.1004}, {14.6479,-1.0454},
		{20.0742,-1.2278}, {21.2886,-1.2977}};

// 802.11ac and 802.11ah
double Standard::min_thresh_ac_ah[10] = {-2.5103, 0.5000, 3.7609, 5.5103, 9.2712,
		12.5206,14.5321,11.6188,15.4106, 15.4106};
double Standard::max_thresh_ac_ah[10] = { 1.9897,  5.0000,  8.2609, 10.5103, 14.7712,
		18.5206, 20.0321, 21.5000, 22.5000, 22.5000};
double Standard::coeff_ac_ah[10][5] = {{  -2.2353000, -1.0721000,-0.1708900, 0.0243860, 0.0096656},
		{  -0.4517200, -0.3560888, 0.0627930,-0.0651410, 0.0064799},
		{  -0.3082200, -0.2063400, 0.1547100,-0.0389730, 0.0018157},
		{   2.6965000, -1.9353000, 0.4736600,-0.0509360, 0.0016224},
		{  34.8692000,-13.9070000, 2.0328000,-0.1283100, 0.0028499},
		{  93.9622000,-26.7075000, 2.8106000,-0.1290100, 0.0021372},
		{-120.1972000, 26.3772000,-2.1564000, 0.0787190,-0.0011189},
		{ -42.9748720, 11.0385340,-1.0249930, 0.0411790,-0.0006080},
		{ -159.279240, 33.9061380,-2.6679000, 0.0922430,-0.0011850},
		{ -159.279240, 33.9061380,-2.6679000, 0.0922430,-0.0011850}};
double Standard::coeff_high_ac_ah[10][2] = {{-2.3974,-1.1580}, { 2.1138,-1.3738}, { 7.7079,-1.5347},
		{ 9.2576,-1.3244}, {11.3789,-1.1004}, {14.6479,-1.0454},
		{20.0742,-1.2278}, {21.2886,-1.2977}, {18.1224,-0.9725},
		{18.1224,-0.9725}};

//////////////////////////////////
// Standard setters and getters //
//////////////////////////////////
void Standard::set_standard(dot11_standard st, channel_bandwidth bw, bool sgi) {
	currentStd = st;
	band = bw;

	switch(st) {
	case dot11a : {
		maxMCS = MCS7;
		maxBand = MHz20;
		break;
	}
	case dot11n : {
		maxMCS = MCS7;
		maxBand = MHz40;
		break;
	}
	case dot11ac: {
		if(band == MHz20) maxMCS = MCS8;
		else maxMCS = MCS9;
		maxBand = MHz160;
		break;
	}
	case dot11ah: {
		maxMCS = MCS9;
		maxBand = MHz160;
		break;
	}
	default : {
		maxMCS = MCS;
		maxBand = MHz20;
	}
	}

	if(bw > maxBand)throw (my_exception("Bandwidth not supported by standard."));

	if(st == dot11ah) symbol_period = 40e-6;
	else symbol_period = 4e-6;

	shortGI = sgi;
	if(shortGI) symbol_period =- 0.4e-6;
}

dot11_standard Standard::get_standard() {
	return currentStd;
}
transmission_mode Standard::get_maxMCS(){
	return maxMCS;
}
double Standard::get_symbol_period(){
	return symbol_period;
}
double Standard::get_min_thresh(int idx) {
	if(currentStd == dot11a) return min_thresh_a[idx];
	else if(currentStd == dot11n) return min_thresh_n[idx];
	else return min_thresh_ac_ah[idx];
}
double Standard::get_max_thresh(int idx) {
	if(currentStd == dot11a) return max_thresh_a[idx];
	else if(currentStd == dot11n) return max_thresh_n[idx];
	else return max_thresh_ac_ah[idx];
}
double Standard::get_coeff(int idx, int i) {
	if(currentStd == dot11a) return coeff_a[idx][i];
	else if(currentStd == dot11n) return coeff_n[idx][i];
	else return coeff_ac_ah[idx][i];
}
double Standard::get_coeff_high(int idx, int i) {
	if(currentStd == dot11a) return coeff_high_a[idx][i];
	else if(currentStd == dot11n) return coeff_high_n[idx][i];
	else return coeff_high_ac_ah[idx][i];
}
channel_bandwidth Standard::get_band() {
	return band;
}
channel_bandwidth Standard::get_maxBand() {
	return maxBand;
}

///////////////////////
// tx_mode_to_double //
///////////////////////
double Standard::tx_mode_to_double (transmission_mode tm) {

	unsigned mode = tm - MCS0;
	unsigned bndW = band - MHz20;
	unsigned sgi = 0;
	if(shortGI) sgi = 1;

	switch(currentStd) {
	case dot11a:  return rates_a[mode];
	case dot11n:  return rates_n[sgi][mode][bndW];
	case dot11ac: return rates_ac_ah[sgi][mode][bndW];
	case dot11ah: return rates_ac_ah[sgi][mode][bndW];
	default: throw (my_exception("Undefined Standard."));
	}
	return 0;
}

////////////////////////////
// txMode_bits_per_symbol //
////////////////////////////
unsigned Standard::txMode_bits_per_symbol(transmission_mode tm) {

	unsigned mode = tm - MCS0;
	unsigned bndW = band - MHz20;

	switch(currentStd) {
	case dot11a:  return bits_per_symb_a[mode];
	case dot11n:  return bits_per_symb_n[mode][bndW];
	case dot11ac: return bits_per_symb_ac_ah[mode][bndW];
	case dot11ah: return bits_per_symb_ac_ah[mode][bndW];
	default: throw (my_exception("Undefined Standard."));
	}
	return 0;
}

////////////////////////
// output operator<<  //
////////////////////////
ostream& operator<< (ostream& os, const dot11_standard& st) {
  switch(st) {
    case dot11a : return os << "802.11a";
    case dot11n : return os << "802.11n";
    case dot11ac: return os << "802.11ac";
    case dot11ah: return os << "802.11ah";
    case dot11  : return os << "dummy standard";
    default: return os << "unknown standard.";
  }
}

///////////////////////
// input operator >> //
///////////////////////
istream& operator>> (istream& is, dot11_standard& st) {
  string str;
  is >> str;

  if (str == "802.11a") st = dot11a;
  else if (str == "802.11n") st = dot11n;
  else if (str == "802.11ac") st = dot11ac;
  else if (str == "802.11ah") st = dot11ah;
  else is.clear(ios::failbit);

  return is;
}

////////////////////////
// output operator<<  //
////////////////////////
ostream& operator<< (ostream& os, const channel_bandwidth& bw) {
  switch(bw) {
    case MHz : return os << "MHz";
    case MHz20 : return os << "20 MHz";
    case MHz40 : return os << "40 MHz";
    case MHz80 : return os << "80 MHz";
    case MHz160: return os << "160 MHz";
    default: return os << "unknown bandwidth.";
  }
}

///////////////////////
// input operator >> //
///////////////////////
istream& operator>> (istream& is, channel_bandwidth& bw) {
  string str;
  is >> str;

  if (str == "20MHz") bw = MHz20;
  else if (str == "40MHz") bw = MHz40;
  else if (str == "80MHz") bw = MHz80;
  else if (str == "160MHz") bw = MHz160;
  else is.clear(ios::failbit);

  return is;
}

