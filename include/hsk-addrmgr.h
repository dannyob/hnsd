#ifndef _HSK_ADDRMGR_H
#define _HSK_ADDRMGR_H

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "hsk-addr.h"
#include "hsk-timedata.h"
#include "hsk-map.h"
#include "msg.h"

typedef struct hsk_addrentry_s {
  int32_t af;
  uint8_t ip[16];
  uint16_t port;
  uint64_t time;
  uint64_t services;
  int32_t attempts;
  int64_t last_success;
  int64_t last_attempt;
  int32_t ref_count;
  bool used;
} hsk_addrentry_t;

typedef struct hsk_banned_t {
  int32_t af;
  uint8_t ip[16];
  uint16_t port;
  int64_t time;
} hsk_banned_t;

typedef struct hsk_addrman_s {
  hsk_timedata_t *td;
  size_t fresh_size;
  hsk_addrentry_t **fresh;
  size_t used_size;
  hsk_addrentry_t **used;
  hsk_map_t map;
  hsk_map_t banned;
} hsk_addrman_t;

int32_t
hsk_addrman_init(hsk_addrman_t *am, hsk_timedata_t *td);

void
hsk_addrman_uninit(hsk_addrman_t *am);

hsk_addrman_t *
hsk_addrman_alloc(void);

void
hsk_addrman_free(hsk_addrman_t *am);

hsk_addrentry_t *
hsk_addrman_alloc_entry(hsk_addrman_t *am);

bool
hsk_addrman_add(hsk_addrman_t *am, hsk_addrentry_t *addr, bool src);

bool
hsk_addrman_add_addr(hsk_addrman_t *am, hsk_netaddr_t *addr);

int32_t
hsk_addrman_add_sa(hsk_addrman_t *am, struct sockaddr *addr);

int32_t
hsk_addrman_add_ip(hsk_addrman_t *am, int32_t af, uint8_t *ip, uint16_t port);

hsk_addrentry_t *entry
hsk_addrman_get_by_key(
  hsk_addrman_t *am,
  int32_t af,
  uint8_t *ip,
  uint16_t port
);

bool
hsk_addrman_mark_attempt(
  hsk_addrman_t *am,
  int32_t af,
  uint8_t *ip,
  uint16_t port
);

bool
hsk_addrman_mark_success(
  hsk_addrman_t *am,
  int32_t af,
  uint8_t *ip,
  uint16_t port
);

bool
hsk_addrman_mark_ack(
  hsk_addrman_t *am,
  int32_t af,
  uint8_t *ip,
  uint16_t port,
  uint64_t services
);

void
hsk_addrman_clear_banned(hsk_addrman_t *am);

bool
hsk_addrman_add_ban(hsk_addrman_t *am, int32_t af, uint8_t *ip);

bool
hsk_addrman_is_banned(hsk_addrman_t *am, int32_t af, uint8_t *ip);

hsk_addrentry_t *
hsk_addrman_pick(hsk_addrman_t *am, hash_map_t *map);

bool
hsk_addrman_pick_sa(hsk_addrman_t *am, hash_map_t *map, struct sockaddr *addr);
#endif
