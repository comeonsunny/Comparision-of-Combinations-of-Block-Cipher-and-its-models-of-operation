#include "Cipher.h"
#include <iostream>

using namespace std;

#define tomcrypt_assert(x) { \
    if (err != CRYPT_OK) { \
        cout << "[Error] " << error_to_string(err) << endl; \
        assert(err == CRYPT_OK); \
    } \
}

EncryptModel::EncryptModel(string name, vector<unsigned char> key) : _cipher_name(name), _key(key)
{
    register_all_ciphers();

    _cipher_index = find_cipher(_cipher_name.c_str());
    assert(_cipher_index != -1);

    int ivsize = cipher_descriptor[_cipher_index].block_length;
    _iv = vector<unsigned char>(ivsize);

    int key_size = _key.size();
    int err = cipher_descriptor[_cipher_index].keysize(&key_size);
    _key.resize(key_size, 'g');
    tomcrypt_assert(err)
}

void CbcModel::encrypt_data(const unsigned char *pt, unsigned char *ct, int len)
{
    int err;

    err = cbc_start(_cipher_index, _iv.data(), _key.data(), _key.size(), 0, &_cbc);
    tomcrypt_assert(err == CRYPT_OK);

    err = cbc_encrypt(pt, ct, len, &_cbc);
    tomcrypt_assert(err == CRYPT_OK);

    err = cbc_done(&_cbc);
    tomcrypt_assert(err == CRYPT_OK);
}

void CbcModel::decrypt_data(const unsigned char *ct, unsigned char *pt, int len)
{
    int err;

    err = cbc_start(_cipher_index, _iv.data(), _key.data(), _key.size(), 0, &_cbc);
    tomcrypt_assert(err == CRYPT_OK);

    err = cbc_decrypt(ct, pt, len, &_cbc);
    tomcrypt_assert(err == CRYPT_OK);

    err = cbc_done(&_cbc);
    tomcrypt_assert(err == CRYPT_OK);
}

void CfbModel::encrypt_data(const unsigned char *pt, unsigned char *ct, int len)
{
    int err;

    err = cfb_start(_cipher_index, _iv.data(), _key.data(), _key.size(), 0, &_cfb);
    tomcrypt_assert(err == CRYPT_OK);

    err = cfb_encrypt(pt, ct, len, &_cfb);
    tomcrypt_assert(err == CRYPT_OK);

    err = cfb_done(&_cfb);
    tomcrypt_assert(err == CRYPT_OK);
}
void CfbModel::decrypt_data(const unsigned char *ct, unsigned char *pt, int len)
{
    int err;

    err = cfb_start(_cipher_index, _iv.data(), _key.data(), _key.size(), 0, &_cfb);
    tomcrypt_assert(err == CRYPT_OK);

    err = cfb_decrypt(ct, pt, len, &_cfb);
    tomcrypt_assert(err == CRYPT_OK);

    err = cfb_done(&_cfb);
    tomcrypt_assert(err == CRYPT_OK);
}

void CtrModel::encrypt_data(const unsigned char *pt, unsigned char *ct, int len)
{
    int err;

    err = ctr_start(_cipher_index, _iv.data(), _key.data(), _key.size(), 0, CTR_COUNTER_LITTLE_ENDIAN, &_ctr);
    tomcrypt_assert(err == CRYPT_OK);

    err = ctr_encrypt(pt, ct, len, &_ctr);
    tomcrypt_assert(err == CRYPT_OK);

    err = ctr_done(&_ctr);
    tomcrypt_assert(err == CRYPT_OK);
}
void CtrModel::decrypt_data(const unsigned char *ct, unsigned char *pt, int len)
{
    int err;

    err = ctr_start(_cipher_index, _iv.data(), _key.data(), _key.size(), 0, CTR_COUNTER_LITTLE_ENDIAN, &_ctr);
    tomcrypt_assert(err == CRYPT_OK);

    err = ctr_decrypt(ct, pt, len, &_ctr);
    tomcrypt_assert(err == CRYPT_OK);

    err = ctr_done(&_ctr);
    tomcrypt_assert(err == CRYPT_OK);
}

void EcbModel::encrypt_data(const unsigned char *pt, unsigned char *ct, int len)
{
    int err;

    err = ecb_start(_cipher_index, _key.data(), _key.size(), 0, &_ecb);
    tomcrypt_assert(err == CRYPT_OK);

    err = ecb_encrypt(pt, ct, len, &_ecb);
    tomcrypt_assert(err == CRYPT_OK);

    err = ecb_done(&_ecb);
    tomcrypt_assert(err == CRYPT_OK);
}
void EcbModel::decrypt_data(const unsigned char *ct, unsigned char *pt, int len)
{
    int err;

    err = ecb_start(_cipher_index, _key.data(), _key.size(), 0, &_ecb);
    tomcrypt_assert(err == CRYPT_OK);

    err = ecb_decrypt(ct, pt, len, &_ecb);
    tomcrypt_assert(err == CRYPT_OK);

    err = ecb_done(&_ecb);
    tomcrypt_assert(err == CRYPT_OK);
}

void OfbModel::encrypt_data(const unsigned char *pt, unsigned char *ct, int len)
{
    int err;

    err = ofb_start(_cipher_index, _iv.data(), _key.data(), _key.size(), 0, &_ofb);
    tomcrypt_assert(err == CRYPT_OK);

    err = ofb_encrypt(pt, ct, len, &_ofb);
    tomcrypt_assert(err == CRYPT_OK);

    err = ofb_done(&_ofb);
    tomcrypt_assert(err == CRYPT_OK);
}
void OfbModel::decrypt_data(const unsigned char *ct, unsigned char *pt, int len)
{
    int err;

    err = ofb_start(_cipher_index, _iv.data(), _key.data(), _key.size(), 0, &_ofb);
    tomcrypt_assert(err == CRYPT_OK);

    err = ofb_decrypt(ct, pt, len, &_ofb);
    tomcrypt_assert(err == CRYPT_OK);

    err = ofb_done(&_ofb);
    tomcrypt_assert(err == CRYPT_OK);
}