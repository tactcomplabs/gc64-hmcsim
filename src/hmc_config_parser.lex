%option noyywrap
%option nounput
%{
/* HMC_CONFIG_PARSER.LEX */

#include <stdlib.h>
#include <stdio.h>
#include "hmc_sim.h"

enum {
  LOOKUP = 0,
  LINK_PHY_POWER,
  LINK_LOCAL_ROUTE_POWER,
  LINK_REMOTE_ROUTE_POWER,
  XBAR_RQST_SLOT_POWER,
  XBAR_RSP_SLOT_POWER,
  XBAR_ROUTE_EXTERN_POWER,
  VAULT_RQST_SLOT_POWER,
  VAULT_RSP_SLOT_POWER,
  VAULT_CTRL_POWER,
  ROW_ACCESS_POWER,
  TECPLOT_OUTPUT
};

int state;
int config_func( int type, char *word );
struct hmcsim_t *lhmc;
%}
%%
\n                          { state = LOOKUP; }
^LINK_PHY_POWER             { state = LINK_PHY_POWER; }
^LINK_LOCAL_ROUTE_POWER     { state = LINK_LOCAL_ROUTE_POWER; }
^LINK_REMOTE_ROUTE_POWER    { state = LINK_REMOTE_ROUTE_POWER; }
^XBAR_RQST_SLOT_POWER       { state = XBAR_RQST_SLOT_POWER; }
^XBAR_RSP_SLOT_POWER        { state = XBAR_RSP_SLOT_POWER; }
^XBAR_ROUTE_EXTERN_POWER    { state = XBAR_ROUTE_EXTERN_POWER; }
^VAULT_RQST_SLOT_POWER      { state = VAULT_RQST_SLOT_POWER; }
^VAULT_RSP_SLOT_POWER       { state = VAULT_RSP_SLOT_POWER; }
^VAULT_CTRL_POWER           { state = VAULT_CTRL_POWER; }
^ROW_ACCESS_POWER           { state = ROW_ACCESS_POWER; }
^TECPLOT_OUTPUT             { state = TECPLOT_OUTPUT; }
[a-zA-Z0-9\/.-]+     { if( state!=LOOKUP) config_func(state, yytext);}
. ;
%%


int hmcsim_lex_config( struct hmcsim_t *hmc, char *config ){
  lhmc = hmc;
  yyin = fopen( config, "r" );
  yylex();
  fclose( yyin );
  yyin = NULL;
  return 0;
}

int
config_func( int type, char *word ){
  int temp_tecplot = 0;
  switch( type ){
    case LINK_PHY_POWER:
      lhmc->power.link_phy = (float)atof(word);
      break;
    case LINK_LOCAL_ROUTE_POWER:
      lhmc->power.link_local_route = (float)atof(word);
      break;
    case LINK_REMOTE_ROUTE_POWER:
      lhmc->power.link_remote_route = (float)atof(word);
      break;
    case XBAR_RQST_SLOT_POWER:
      lhmc->power.xbar_rqst_slot = (float)atof(word);
      break;
    case XBAR_RSP_SLOT_POWER:
      lhmc->power.xbar_rsp_slot = (float)atof(word);
      break;
    case XBAR_ROUTE_EXTERN_POWER:
      lhmc->power.xbar_route_extern = (float)atof(word);
      break;
    case VAULT_RQST_SLOT_POWER:
      lhmc->power.vault_rqst_slot = (float)atof(word);
      break;
    case VAULT_RSP_SLOT_POWER:
      lhmc->power.vault_rsp_slot = (float)atof(word);
      break;
    case VAULT_CTRL_POWER:
      lhmc->power.vault_ctrl = (float)atof(word);
      break;
    case ROW_ACCESS_POWER:
      lhmc->power.row_access = (float)atof(word);
      break;
    case TECPLOT_OUTPUT:
      temp_tecplot = atoi(word);
      if( temp_tecplot != 0 ){
        lhmc->power.tecplot = 1;
      }else{
        lhmc->power.tecplot = 0;
      }
      break;
    default:
    break;
  }
  return 0;
}
