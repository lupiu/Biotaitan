idf_component_register(SRCS "lcm.c"
                            "../spi_api/spi_api.c"
                       INCLUDE_DIRS "."
                                    "../spi_api"
                       REQUIRES "driver"
)
