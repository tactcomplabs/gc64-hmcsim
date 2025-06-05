target_sources(hmcsim PUBLIC FILE_SET hmcsim_headers TYPE HEADERS
    FILES hmc_sim.h hmc_sim_macros.h hmc_sim_tecplot.h hmc_sim_types.h)
install(TARGETS hmcsim FILE_SET hmcsim_headers)
