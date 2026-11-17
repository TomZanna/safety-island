/**
 * @file flow_mem.h
 * @brief Helper di memoria del sottosistema flow.
 * @ingroup flow
 *
 * Il codice safe non dipende dalla libc: con FLOW_FREESTANDING=1 (default 0 per
 * queste app, che linkano newlib-nano) fornisce anche i simboli memcpy/memset/
 * memcmp che il compilatore puo' emettere.
 */
#ifndef SI_FLOW_MEM_H
#define SI_FLOW_MEM_H

#include "flow_types.h"

/** @brief Copia `size` byte (equivalente di memcpy, senza libc).
 *
 * @param[out] destination area di destinazione, almeno `size` byte;
 * @param[in]  source      area sorgente, almeno `size` byte;
 * @param[in]  size        numero di byte da copiare.
 * @pre Le due aree non si sovrappongono (non e' memmove).
 */
void flow_memcpy(void* destination, const void* source, flow_u32 size);

/** @brief Riempie `size` byte con `value`.
 *
 * @param[out] destination area da riempire;
 * @param[in]  value        byte da scrivere;
 * @param[in]  size         numero di byte da scrivere.
 */
void flow_memset(void* destination, flow_u8 value, flow_u32 size);

/** @brief Confronto lessicografico a byte (equivalente di memcmp).
 *
 * @param[in] left  prima area;
 * @param[in] right seconda area;
 * @param[in] size  numero di byte da confrontare.
 * @return <0, 0, >0 se `left` e' rispettivamente minore, uguale o maggiore.
 */
int flow_memcmp(const void* left, const void* right, flow_u32 size);

#endif /* SI_FLOW_MEM_H */
