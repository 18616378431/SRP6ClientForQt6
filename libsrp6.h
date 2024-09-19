#ifndef LIBSRP6_H
#define LIBSRP6_H

#pragma once

#include <string>
#include "BigNum.h"

class srp6_caculate {
public:
    srp6_caculate(BigNum N, BigNum g, BigNum s, BigNum B, std::string username, std::string password);
    srp6_caculate(BigNum N, BigNum g, BigNum s, BigNum B, std::string username, std::string password, BigNum a);

    BigNum calculate_session_key();
    BigNum calculate_proof();
    unsigned char * calculate_crc_hash(std::string versionHash);
    unsigned char * calculate_M2();

    BigNum a, k;
    BigNum N, g, B, s;
    BigNum x, u, K, A, S, M;
    std::string username, password;
    BigNum M2;
    BigNum crc_hash;

private:
    void calculate_private_ephemeral();
    void calculate_public_ephemeral();
    void calculate_x();
    void calculate_u();
    void calculate_S();
    void calculate_interleaved();
    unsigned char* calculate_xor_hash();
    unsigned char* calculate_username_hash();
};

#endif
