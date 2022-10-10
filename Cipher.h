#ifndef _CIPHER_H
#define _CIPHER_H

#include <string>
#include <vector>
#include <tomcrypt.h>

class EncryptModelVirtual
{
public:
    virtual void encrypt_data(const unsigned char *pt, unsigned char *ct, int size) = 0;
    virtual void decrypt_data(const unsigned char *ct, unsigned char *pt, int size) = 0;
};

class EncryptModel : public EncryptModelVirtual
{
public:
    EncryptModel(std::string name, std::vector<unsigned char> key);
    std::string _cipher_name;
    int _cipher_index;
    std::vector<unsigned char> _key;
    std::vector<unsigned char> _iv;
};

class CbcModel : public EncryptModel
{
public:
    CbcModel(std::string name, std::vector<unsigned char> key) : EncryptModel(name, key)
    { }
    virtual void encrypt_data(const unsigned char *pt, unsigned char *ct, int len) override;
    virtual void decrypt_data(const unsigned char *ct, unsigned char *pt, int len) override;
private:
    symmetric_CBC _cbc;
};

class CfbModel : public EncryptModel
{
public:
    CfbModel(std::string name, std::vector<unsigned char> key) : EncryptModel(name, key)
    { }
    virtual void encrypt_data(const unsigned char *pt, unsigned char *ct, int len) override;
    virtual void decrypt_data(const unsigned char *ct, unsigned char *pt, int len) override;
private:
    symmetric_CFB _cfb;
};

class CtrModel : public EncryptModel
{
public:
    CtrModel(std::string name, std::vector<unsigned char> key) : EncryptModel(name, key)
    { }
    virtual void encrypt_data(const unsigned char *pt, unsigned char *ct, int len) override;
    virtual void decrypt_data(const unsigned char *ct, unsigned char *pt, int len) override;
private:
    symmetric_CTR _ctr;
};

class EcbModel : public EncryptModel
{
public:
    EcbModel(std::string name, std::vector<unsigned char> key) : EncryptModel(name, key)
    { }
    virtual void encrypt_data(const unsigned char *pt, unsigned char *ct, int len) override;
    virtual void decrypt_data(const unsigned char *ct, unsigned char *pt, int len) override;
private:
    symmetric_ECB _ecb;
};

class OfbModel : public EncryptModel
{
public:
    OfbModel(std::string name, std::vector<unsigned char> key) : EncryptModel(name, key)
    { }
    virtual void encrypt_data(const unsigned char *pt, unsigned char *ct, int len) override;
    virtual void decrypt_data(const unsigned char *ct, unsigned char *pt, int len) override;
private:
    symmetric_OFB _ofb;
};

#endif