/*
 * self_test.h
 *
 * Startup self-test (known answer tests) of the safety mechanisms that are
 * shared by the two cores: the digest used by every cross-check, the frame
 * split/assembly rules and the safe logic.
 *
 * It runs once at the beginning of the initialisation, before the SHMEM
 * mailbox is created and before any thread is started: if it fails the system
 * must not start at all (fail-safe).
 */

#ifndef SELF_TEST_H
#define SELF_TEST_H

#include <stdbool.h>

/** \brief Run all the startup self-tests. Returns false on any failure. */
bool flow_self_test_run (void);

#endif /* SELF_TEST_H */
