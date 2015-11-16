#ifndef _link_adapt_h
#define _link_adapt_h 1

#include "link_adapt_private.h"


////////////////////////////////////////////////////////////////////////////////
// struct link_adapt                                                          //
//                                                                            //
// link adaptation parameters                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
struct adapt_struct {
  transmission_mode mode;                // transmission mode (see Packet.h)
  adapt_mode        adapt;               // what is to be adapted
  double            power_max;           // maximum transmission power in dBm
  double            power_min;           // minimum transmission power in dBm
  double            power_step_down;     // power adaptation step in dBm
  double            power_step_up;       // power adaptation step in dBm
  double            per;                 // target packet error rate 
                                         //(if mode == OPT)

  unsigned          max_succeed_counter; // maximum succeed counter
  unsigned          la_fail_limit;       // maximum fail counter
  bool              use_rx_mode;         // use data rate of received packets?
                                         // (if mode == SUBOPT)

  adapt_struct(transmission_mode m, adapt_mode am, double pmax, double pmin,
               double psu, double psd, double e, unsigned msc, unsigned lfl,
               bool urm) 
               : mode(m), adapt(am), power_max(pmax), power_min(pmin),
                 power_step_up(psu), power_step_down(psd), per(e),
                 max_succeed_counter(msc), la_fail_limit(lfl), use_rx_mode(urm)
                 {}
};

////////////////////////////////////////////////////////////////////////////////
// class link_adapt                                                           //
//                                                                            //
// implements the link adaptation in IEEE 802.11a                             //
// currently only the transmission rate can be adapted.                       //
//                                                                            //
//                                                                            //
// Usage:                                                                     //
//                                                                            //
// the member functions "get_current_mode" and "get_power" return the current //
// adaptation parameters (transmission rate and power) depending on the       //
// algorithm selected with the constructor. The following algorithms are      //
// supported:                                                                 //
// - fixed data rate                                                          //
// - optimal adaptation, the transmitter knows the channel and select the     //
//   highest data rate such that a given PER is not exceeded.                 //
// - suboptimal adaptation, transmitter-based algorithm relying on whether    //
//   ACKs are received or not.                                                //
//                                                                            //
// The functions "failed", "success" and "rx_success" are called by the       //
// terminal respectively if a transmission fails (ACK timeout), if a          //
// transmission is successfull (ACK received) or if a data packet is received //
// is received correctly. These functions are only useful if the suboptimal   //
// adaptation algorithm is employed, they are ignored otherwise.              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
class link_adapt : link_adapt_private {

public:
  link_adapt() {};
  link_adapt(Terminal* from,      // link between terminal *from
             Terminal* to,        // and *to
             adapt_struct param,  // link adaptation parameters
             log_file* l          // pointer to log class
             );

  transmission_mode get_current_mode(unsigned pl);
  // return transmission data rate
  
  double get_power(unsigned pl);
  // return transmission power (in dBm)

  void failed(); 
  // ACK timed out, transmission failed, adapt link
  
  void rts_failed(); 
  // CTS timed out, transmission failed, adapt link
  
  void rx_success(transmission_mode rx_mode);
  // data packet received successfully with data rate 'rx_mode', adapt link

  void success(bool lastfrag);
  // ACK received, transmission succeeded, adapt link.
  // Adaptive link parameters may change only with last fragment, i.e., if 
  // "lastfrag" is true.
};

#endif
