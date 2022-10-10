#include <iostream>
#include <assert.h>
#include <string.h>
#include <sys/time.h>
#include <tomcrypt.h>
#include "Cipher.h"

using namespace std;

static int usage(const char *name);
static std::pair<long, long> test(EncryptModel *cipher, int block_size);

int main(int argc, char *argv[])
{
    string cipher_name;
    string encrypt_model;
    int block_size;
    vector<unsigned char> key;

    // 参数处理
    if (argc > 4)
    {
        cipher_name = argv[1];
        encrypt_model = argv[2];
        key = vector<unsigned char>(atoi(argv[3]), 'g');
        block_size = atoi(argv[4]);
    }
    else if (argc == 2 && strcmp(argv[1], "test") == 0)
    {
        cipher_name = "aes";
        encrypt_model = "ofb";
        block_size = 512;
        key = vector<unsigned char>(16, 'g');
    }
    else
    {
        usage(argv[0]);
    }

    EncryptModel *cipher;
    if (encrypt_model == "cbc")
        cipher = new CbcModel(cipher_name, key);
    else if (encrypt_model == "cfb")
        cipher = new CfbModel(cipher_name, key);
    else if (encrypt_model == "ctr")
        cipher = new CtrModel(cipher_name, key);
    else if (encrypt_model == "ecb")
        cipher = new EcbModel(cipher_name, key);
    else if (encrypt_model == "ofb")
        cipher = new OfbModel(cipher_name, key);

    std::pair<long, long> time = test(cipher, block_size);
    cout << cipher_name << " " << encrypt_model << " " << key.size() << " " << block_size << " "
        << time.first << " " << time.second << endl;
}

static int usage(const char *name)
{
    cout << "Usage: " << name << " <cipher> <model> <key size> <block size>" << endl;

    register_all_ciphers();
    cout << "cipher: [";
    for (int x = 0; cipher_descriptor[x].name != NULL; x++)
        cout << " " << cipher_descriptor[x].name;
    cout << " ]" << endl;

    cout << "model: [ cbc cfb ctr ecb ofb ]" << endl;

    exit(1);
}
static std::pair<long, long> test(EncryptModel *cipher, int block_size)
{
    struct timeval start, end;
    long encrypt_time, decrypt_time;
    unsigned char *data = new unsigned char[block_size];
    unsigned char *pt = new unsigned char[block_size];

    // encrypt
    gettimeofday(&start, NULL);
    cipher->encrypt_data(pt, pt, block_size);
    gettimeofday(&end, NULL);
    encrypt_time = 1000000 * (end.tv_sec-start.tv_sec) + (end.tv_usec-start.tv_usec);

    // decrypt
    gettimeofday(&start, NULL);
    cipher->decrypt_data(pt, pt, block_size);
    gettimeofday(&end, NULL);
    decrypt_time = 1000000 * (end.tv_sec-start.tv_sec) + (end.tv_usec-start.tv_usec);

    assert(memcmp(pt, data, block_size) == 0);
    return pair<long, long>(encrypt_time, decrypt_time);
}