#ifdef __APPLE__
 #include <stdbool.h>
#endif

#include <v8.h>
#include <mhash.h>
#include <node.h>
#include <node_buffer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace v8;
using namespace node;

void reverse_bytes(unsigned char * start, unsigned long len)
{
	unsigned char *lo = start;
	unsigned char *hi = start + len - 1;
	unsigned char swap;
	while(lo<hi)
	{
		swap = *lo;
		*lo++ = *hi;
		*hi-- = swap;
	}
}

char * convert_to_hex(unsigned char * hash_data, unsigned long len)
{
	char *			converted;
	unsigned long	i;

	converted = (char *)malloc((len*2)+1);
	memset(converted, 0, (len*2)+1);

	for(i=0;i<len;i++)
	{
		sprintf(converted+(i*2), "%02x", hash_data[i]);
	}

	return converted;
}

char * hash(hashid hashType, unsigned char * data, unsigned long len)
{
	MHASH				td;
	unsigned char * 	hash_data;
	char *				converted=0;
	unsigned long		block_size;

	if((td=mhash_init(hashType))==MHASH_FAILED)
		return 0;

	mhash(td, data, len);

	if((hash_data=(unsigned char *)mhash_end(td)))
	{
		block_size = mhash_get_block_size(hashType);
		if(hashType==MHASH_ADLER32)	// Fixes #6
			reverse_bytes(hash_data, block_size);
		converted = convert_to_hex(hash_data, block_size);
		mhash_free(hash_data);
	}

	return converted;
}

hashid get_hash_type_by_name(char * name)
{
	if(strcasecmp(name, "crc32")==0)
		return MHASH_CRC32;
	else if(strcasecmp(name, "crc32b")==0)
		return MHASH_CRC32B;
	else if(strcasecmp(name, "md2")==0)
		return MHASH_MD2;
	else if(strcasecmp(name, "md4")==0)
		return MHASH_MD4;
	else if(strcasecmp(name, "md5")==0)
		return MHASH_MD5;
	else if(strcasecmp(name, "haval128")==0)
		return MHASH_HAVAL128;
	else if(strcasecmp(name, "haval160")==0)
		return MHASH_HAVAL160;
	else if(strcasecmp(name, "haval192")==0)
		return MHASH_HAVAL192;
	else if(strcasecmp(name, "haval224")==0)
		return MHASH_HAVAL224;
	else if(strcasecmp(name, "haval256")==0)
		return MHASH_HAVAL256;
	else if(strcasecmp(name, "sha1")==0)
		return MHASH_SHA1;
	else if(strcasecmp(name, "sha224")==0)
		return MHASH_SHA224;
	else if(strcasecmp(name, "sha256")==0)
		return MHASH_SHA256;
	else if(strcasecmp(name, "sha384")==0)
		return MHASH_SHA384;
	else if(strcasecmp(name, "sha512")==0)
		return MHASH_SHA512;
	else if(strcasecmp(name, "ripemd128")==0)
		return MHASH_RIPEMD128;
	else if(strcasecmp(name, "ripemd160")==0)
		return MHASH_RIPEMD160;
	else if(strcasecmp(name, "ripemd256")==0)
		return MHASH_RIPEMD256;
	else if(strcasecmp(name, "ripemd320")==0)
		return MHASH_RIPEMD320;
	else if(strcasecmp(name, "tiger128")==0)
		return MHASH_TIGER128;
	else if(strcasecmp(name, "tiger160")==0)
		return MHASH_TIGER160;
	else if(strcasecmp(name, "tiger192")==0)
		return MHASH_TIGER192;
	else if(strcasecmp(name, "gost")==0)
		return MHASH_GOST;
	else if(strcasecmp(name, "whirlpool")==0)
		return MHASH_WHIRLPOOL;
	else if(strcasecmp(name, "adler32")==0)
		return MHASH_ADLER32;
	else if(strcasecmp(name, "snefru128")==0)
		return MHASH_SNEFRU128;
	else if(strcasecmp(name, "snefru256")==0)
		return MHASH_SNEFRU256;

	return (hashid)-1;
}

Handle<Value> hash_binding(const Arguments& args)
{
	HandleScope 	scope;
	Local<String> 	ret;
	char *			hashed=0;
	hashid			type=(hashid)-1;

	String::Utf8Value 	name(args[0]->ToString());
	type = get_hash_type_by_name(*name);
	if(type==(hashid)-1)
		return Null();

	if(args[1]->IsString())
	{
		String::Utf8Value 	data(args[1]->ToString());
		hashed = hash(type, (unsigned char *)*data, data.length());
	}
	else if(Buffer::HasInstance(args[1]))
	{
		Handle<Object> data = args[1]->ToObject();
		hashed = hash(type, (unsigned char *)Buffer::Data(data), Buffer::Length(data));
	}

	if(!hashed)
		return Null();

	ret = String::New(hashed);
	free(hashed);

	return scope.Close(ret);
}

extern "C" void init(Handle<Object> target)
{
	HandleScope		scope;

	target->Set(String::New("hash"), FunctionTemplate::New(hash_binding)->GetFunction());
}

#ifdef NODE_MODULE
    NODE_MODULE(mhash, init)
#endif
