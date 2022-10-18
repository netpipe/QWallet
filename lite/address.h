//https://www.reddit.com/r/Bitcoin/comments/1dact6/looking_to_generate_addresses_of_my_own_where_to/

//http://www.certicom.com/images/pdfs/WP-ECCprimer.pdf

//http://www.certicom.com/index.php/ecc-tutorial

//gcc address.cpp -fpermissive -lssl -lboost_system -lcrypto -o test

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <openssl/sha.h>
#include <openssl/ssl.h>
#include <openssl/ec.h>
#include <openssl/ripemd.h>
#include <memory.h>

int create_address_from_string(const unsigned char *string,
		unsigned char *address,
		unsigned char *priv_key,
		bool base58,
		bool debug,
		EC_GROUP *precompgroup);
void print_hex(u_int8_t * buffer, unsigned int len);
void base58_encode(unsigned char *data, unsigned int len, char *result);
void prepare_for_address(unsigned char *data, int datalen, char start_byte);


int generateAddress() {
    unsigned char message[32] = "password";
    unsigned char address[64];
    unsigned char priv_key[64];

    if (create_address_from_string(message, address, priv_key, true, false, NULL) == 1) {
		printf("address: %s\n", address);
		printf("private key: %s\n", priv_key);
		return 0;
	}
	else {
		printf("Something went wront :(\n");
		return 1;
	}
}

/* creates a bitcoin address+private key from the SHA256
 *  hash of string. converts to base58 if base58 is 'true'
 *  returns 1 if successful, 0 if not*/
int create_address_from_string(const unsigned char *string,
		unsigned char *address,
		unsigned char *priv_key,
		bool base58,
		bool debug,
		EC_GROUP *precompgroup) {

    u_int8_t * hash = malloc(SHA256_DIGEST_LENGTH);
    BIGNUM * n = BN_new();

    //first we hash the string
    SHA256 (string, strlen(string), hash);
	//then we convert the hash to the BIGNUM n
    n = BN_bin2bn(hash, SHA256_DIGEST_LENGTH, n);

    BIGNUM * order = BN_new();
    BIGNUM * nmodorder = BN_new();
	BN_CTX *bnctx;
	bnctx = BN_CTX_new();

    //then we create a new EC group with the curve secp256k1
	EC_GROUP * pgroup;
	if (precompgroup == NULL)
		pgroup = EC_GROUP_new_by_curve_name(NID_secp256k1);
	else
		//unless one has been passed to this function. in which case we use that curve.
		pgroup = precompgroup;


    if (!pgroup) {
    	printf("ERROR: Couldn't get new group\n");
    	return 0;
    }

    //now we need to get the order of the group, and make sure that
    //the number we use for the private key is less than or equal to
    //the group order by using "nmodorder = n % order"
    EC_GROUP_get_order(pgroup, order, NULL);
    BN_mod(nmodorder, n, order, bnctx);

    if (BN_is_zero(nmodorder)) {
    	printf("ERROR: SHA256(string) %% order == 0. Pick another string.\n");
    	return 0;
    }

    //now we create a new EC point, ecpoint, and place in it the secp256k1
    //generator point multiplied by nmodorder. this newly created
    //point is the public key


    EC_POINT * ecpoint = EC_POINT_new(pgroup);

	if (!EC_POINT_mul(pgroup, ecpoint, nmodorder, NULL, NULL, NULL))
	{
    	printf("ERROR: Couldn't multiply the generator point with n\n");
    	return 0;
    }

    if (debug) {
        BIGNUM *x=NULL, *y=NULL;
        x=BN_new();
        y=BN_new();

        if (!EC_POINT_get_affine_coordinates_GFp(pgroup, ecpoint, x, y, NULL)) {
        	printf("ERROR: Failed getting coordinates.");
        	return 0;
        }

    	printf ("x: %s, y: %s\n", BN_bn2dec(x), BN_bn2dec(y));

        BN_free(x);
        BN_free(y);
    }

    //then we need to convert the public key point to data
    //first we get the required size of the buffer in which the data is placed
    //by passing NULL as the buffer argument to EC_POINT_point2oct
    unsigned int bufsize = EC_POINT_point2oct (pgroup, ecpoint, POINT_CONVERSION_UNCOMPRESSED, NULL, 0, NULL);
    u_int8_t * buffer = malloc(bufsize);
    //then we place the data in the buffer
    int len = EC_POINT_point2oct (pgroup, ecpoint, POINT_CONVERSION_UNCOMPRESSED, buffer, bufsize, NULL);
    if (len == 0) {
    	printf("ERROR: Couldn't convert point to octet string.");
    	return 0;
    }

    //next we need to hash the public key data. first with SHA256, then with RIPEMD160
    SHA256(buffer, len, hash);
    u_int8_t * ripemd = malloc(RIPEMD160_DIGEST_LENGTH+1+4);
    RIPEMD160(hash, SHA256_DIGEST_LENGTH, ripemd);

    if (base58 == true) {
		//here we add the version byte to the beginning of the public key and four checksum
		//bytes at the end
		prepare_for_address(ripemd, RIPEMD160_DIGEST_LENGTH, 0);


		if (debug)
			print_hex(ripemd, RIPEMD160_DIGEST_LENGTH+1+4);

		//and we convert the resulting data to base58
		base58_encode(ripemd, RIPEMD160_DIGEST_LENGTH+1+4, address);
    } else {
    	memcpy(address, ripemd, RIPEMD160_DIGEST_LENGTH);
    }


    //now we need to convert the big number nmodorder (private key) to data
    int buflen = BN_num_bytes(nmodorder);
    u_int8_t * buf = malloc(buflen+1+4);
    int datalen;

    //nmodorder is converted to binary representation
    datalen = BN_bn2bin(nmodorder, buf);


    if (base58 == true) {
		//and we add version byte and four byte checksum to the data
		prepare_for_address(buf, datalen, 0x80);

        //and convert this to base58
        base58_encode(buf, datalen+5, priv_key);
    } else {
    	memcpy(priv_key, buf, datalen+5);
    }

    free(hash);
    free(buffer);
    free(ripemd);
    free(buf);
    BN_free(n);
    BN_free(order);
    BN_free(nmodorder);
    if (precompgroup == NULL)
    	EC_GROUP_free(pgroup);
    EC_POINT_free(ecpoint);
    BN_CTX_free(bnctx);
    return 1;
}

/*prepares data to be converted to address. specifically, it adds
 start_byte to the beginning and a four-byte doubleSHA256 checksum to the end */
void prepare_for_address(unsigned char *data, int datalen, char start_byte) {
	unsigned char *tmpbuf = malloc(datalen);
    //get data into a temporary buffer
    memcpy(tmpbuf, data, datalen);
    //shift data one byte forward, to make room for star_byte
    memcpy(data+1, tmpbuf, datalen);
    data[0] = start_byte;

    unsigned char *hash = malloc(SHA256_DIGEST_LENGTH);
    SHA256(data, datalen+1, hash);
    SHA256(hash, SHA256_DIGEST_LENGTH, hash);

    //copy four first bytes from hash to the end of data (checksum bytes)
    memcpy(data+datalen+1, hash, 4);
    free(tmpbuf);
    free(hash);
}

void print_hex(u_int8_t * buffer, unsigned int len) {
	int x;
	for (x = 0; x < len; x++) {
		printf("%.2x",buffer[x]);
	}
	printf("\n");
}

/*return the base58 encoding of data*/
void base58_encode(unsigned char *data, unsigned int len, char *result) {
	const char code_string[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

    BIGNUM *x, *rem, *base, *tmp, *tmp2;
    x = BN_new();
	rem = BN_new();
	base = BN_new();
	tmp = BN_new();
	char * output_string = malloc(64);

	x = BN_bin2bn(data, len, x);

	BN_set_word(rem, 1);
	BN_set_word(base, 58);

	BN_CTX *bnctx;
	bnctx = BN_CTX_new();

	int i = 0;
	while (!BN_is_zero(x)) {
		BN_div(tmp, rem, x, base, bnctx);
		output_string[i++] = code_string[BN_get_word(rem)];


		tmp2 = x;
		x = tmp;
		tmp = tmp2;
	}


	//public key
	int i2 = 0;
	while (data[i2] == 0) {
		output_string[i++] = code_string[0];
		i2++;
	}

	int base58len = i;
	while (i>=0) {
		result[base58len-i] = output_string[i-1];
		i--;
	}
	result[base58len] = 0;

	BN_free(x);
	BN_free(base);
	BN_free(rem);
	BN_free(tmp);
	BN_CTX_free(bnctx);
	free(output_string);
}
