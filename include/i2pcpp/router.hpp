#ifndef I2PAPI_HPP
#define I2PAPI_HPP

#define I2PAPI_VERSION 0

#ifdef __cplusplus
extern "C" {
#endif

void i2p_init();
void *i2p_router_new(char *dbFile);
void i2p_router_free(void *router);
void i2p_db_create(char *dbFile);
void i2p_db_config_set(void *router, char *name, char *value);
char *i2p_db_config_get(void *router, char *name);

#ifdef __cplusplus
}
#endif

#endif