#include <clib/des_protos.h>

__asm __saveds const char *des_options_AmiSSL(void)
{
	return(DES_options());
}

__asm __saveds void des_ecb3_encrypt_AmiSSL(register __a0 const_DES_cblock *input, register __a1 DES_cblock *output, register __d0 DES_key_schedule *ks1, register __d1 DES_key_schedule *ks2, register __d2 DES_key_schedule *ks3, register __d3 int enc)
{
	DES_ecb3_encrypt(input, output, ks1, ks2, ks3, enc);
}

__asm __saveds DES_LONG des_cbc_cksum_AmiSSL(register __a0 const unsigned char *input, register __a1 DES_cblock *output, register __d0 long length, register __d1 DES_key_schedule *schedule, register __a2 const_DES_cblock *ivec)
{
	return(DES_cbc_cksum(input, output, length, schedule, ivec));
}

__asm __saveds void des_cbc_encrypt_AmiSSL(register __a0 const unsigned char *input, register __a1 unsigned char *output, register __d0 long length, register __d1 DES_key_schedule *schedule, register __a2 DES_cblock *ivec, register __d2 int enc)
{
	DES_cbc_encrypt(input, output, length, schedule, ivec, enc);
}

__asm __saveds void des_ncbc_encrypt_AmiSSL(register __a0 const unsigned char *input, register __a1 unsigned char *output, register __d0 long length, register __d1 DES_key_schedule *schedule, register __a2 DES_cblock *ivec, register __d2 int enc)
{
	DES_ncbc_encrypt(input, output, length, schedule, ivec, enc);
}

__asm __saveds void des_xcbc_encrypt_AmiSSL(register __a0 const unsigned char *input, register __a1 unsigned char *output, register __d0 long length, register __d1 DES_key_schedule *schedule, register __a2 DES_cblock *ivec, register __a3 const_DES_cblock *inw, register __d2 const_DES_cblock *outw, register __d3 int enc)
{
	DES_xcbc_encrypt(input, output, length, schedule, ivec, inw, outw, enc);
}

__asm __saveds void des_cfb_encrypt_AmiSSL(register __a0 const unsigned char *in, register __a1 unsigned char *out, register __d0 int numbits, register __d1 long length, register __d2 DES_key_schedule *schedule, register __a2 DES_cblock *ivec, register __d3 int enc)
{
	DES_cfb_encrypt(in, out, numbits, length, schedule, ivec, enc);
}

__asm __saveds void des_ecb_encrypt_AmiSSL(register __a0 const_DES_cblock *input, register __a1 DES_cblock *output, register __d0 DES_key_schedule *ks, register __d1 int enc)
{
	DES_ecb_encrypt(input, output, ks, enc);
}

__asm __saveds void des_encrypt1_AmiSSL(register __a0 DES_LONG *data, register __d0 DES_key_schedule *ks, register __d1 int enc)
{
	DES_encrypt1(data, ks, enc);
}

__asm __saveds void des_encrypt2_AmiSSL(register __a0 DES_LONG *data, register __d0 DES_key_schedule *ks, register __d1 int enc)
{
	DES_encrypt2(data, ks, enc);
}

__asm __saveds void des_encrypt3_AmiSSL(register __a0 DES_LONG *data, register __d0 DES_key_schedule *ks1, register __d1 DES_key_schedule *ks2, register __d2 DES_key_schedule *ks3)
{
	DES_encrypt3(data, ks1, ks2, ks3);
}

__asm __saveds void des_decrypt3_AmiSSL(register __a0 DES_LONG *data, register __d0 DES_key_schedule *ks1, register __d1 DES_key_schedule *ks2, register __d2 DES_key_schedule *ks3)
{
	DES_decrypt3(data, ks1, ks2, ks3);
}

__asm __saveds void des_ede3_cbc_encrypt_AmiSSL(register __a0 const unsigned char *input, register __a1 unsigned char *output, register __d0 long length, register __d1 DES_key_schedule *ks1, register __d2 DES_key_schedule *ks2, register __d3 DES_key_schedule *ks3, register __a2 DES_cblock *ivec, register __d4 int enc)
{
	DES_ede3_cbc_encrypt(input, output, length, ks1, ks2, ks3, ivec, enc);
}

__asm __saveds void des_ede3_cbcm_encrypt_AmiSSL(register __a0 const unsigned char *in, register __a1 unsigned char *out, register __d0 long length, register __d1 DES_key_schedule *ks1, register __d2 DES_key_schedule *ks2, register __d3 DES_key_schedule *ks3, register __a2 DES_cblock *ivec1, register __a3 DES_cblock *ivec2, register __d4 int enc)
{
	DES_ede3_cbcm_encrypt(in, out, length, ks1, ks2, ks3, ivec1, ivec2, enc);
}

__asm __saveds void des_ede3_cfb64_encrypt_AmiSSL(register __a0 const unsigned char *in, register __a1 unsigned char *out, register __d0 long length, register __d1 DES_key_schedule *ks1, register __d2 DES_key_schedule *ks2, register __d3 DES_key_schedule *ks3, register __a2 DES_cblock *ivec, register __a3 int *num, register __d4 int enc)
{
	DES_ede3_cfb64_encrypt(in, out, length, ks1, ks2, ks3, ivec, num, enc);
}

__asm __saveds void des_ede3_ofb64_encrypt_AmiSSL(register __a0 const unsigned char *in, register __a1 unsigned char *out, register __d0 long length, register __d1 DES_key_schedule *ks1, register __d2 DES_key_schedule *ks2, register __d3 DES_key_schedule *ks3, register __a2 DES_cblock *ivec, register __a3 int *num)
{
	DES_ede3_ofb64_encrypt(in, out, length, ks1, ks2, ks3, ivec, num);
}

__asm __saveds void des_xwhite_in2out_AmiSSL(register __a0 const_DES_cblock *des_key, register __a1 const_DES_cblock *in_white, register __a2 DES_cblock *out_white)
{
	DES_xwhite_in2out(des_key, in_white, out_white);
}

__asm __saveds char *des_fcrypt_AmiSSL(register __a0 const char *buf, register __a1 const char *salt, register __a2 char *ret)
{
	return(DES_fcrypt(buf, salt, ret));
}

__asm __saveds char *des_crypt_AmiSSL(register __a0 const char *buf, register __a1 const char *salt)
{
	return(DES_crypt(buf, salt));
}

__asm __saveds void des_ofb_encrypt_AmiSSL(register __a0 const unsigned char *in, register __a1 unsigned char *out, register __d0 int numbits, register __d1 long length, register __d2 DES_key_schedule *schedule, register __a2 DES_cblock *ivec)
{
	DES_ofb_encrypt(in, out, numbits, length, schedule, ivec);
}

__asm __saveds void des_pcbc_encrypt_AmiSSL(register __a0 const unsigned char *input, register __a1 unsigned char *output, register __d0 long length, register __d1 DES_key_schedule *schedule, register __a2 DES_cblock *ivec, register __d2 int enc)
{
	DES_pcbc_encrypt(input, output, length, schedule, ivec, enc);
}

__asm __saveds DES_LONG des_quad_cksum_AmiSSL(register __a0 const unsigned char *input, register __a1 DES_cblock *output, register __d0 long length, register __d1 int out_count, register __a2 DES_cblock *seed)
{
	return(DES_quad_cksum(input, output, length, out_count, seed));
}

__asm __saveds int des_random_key_AmiSSL(register __a0 DES_cblock *ret)
{
	return(DES_random_key(ret));
}

__asm __saveds int des_read_password_AmiSSL(register __a0 DES_cblock *key, register __a1 const char *prompt, register __d0 int verify)
{
	return(DES_read_password(key, prompt, verify));
}

__asm __saveds int des_read_2passwords_AmiSSL(register __a0 DES_cblock *key1, register __a1 DES_cblock *key2, register __a2 const char *prompt, register __d0 int verify)
{
	return(DES_read_2passwords(key1, key2, prompt, verify));
}

__asm __saveds void des_set_odd_parity_AmiSSL(register __a0 DES_cblock *key)
{
	DES_set_odd_parity(key);
}

__asm __saveds int des_check_key_parity_AmiSSL(register __a0 const_DES_cblock *key)
{
	return(DES_check_key_parity(key));
}

__asm __saveds int des_is_weak_key_AmiSSL(register __a0 const_DES_cblock *key)
{
	return(DES_is_weak_key(key));
}

__asm __saveds int des_set_key_AmiSSL(register __a0 const_DES_cblock *key, register __d0 DES_key_schedule *schedule)
{
	return(DES_set_key(key, schedule));
}

__asm __saveds int des_key_sched_AmiSSL(register __a0 const_DES_cblock *key, register __d0 DES_key_schedule *schedule)
{
	return(DES_key_sched(key, schedule));
}

__asm __saveds int des_set_key_checked_AmiSSL(register __a0 const_DES_cblock *key, register __d0 DES_key_schedule *schedule)
{
	return(DES_set_key_checked(key, schedule));
}

__asm __saveds void des_set_key_unchecked_AmiSSL(register __a0 const_DES_cblock *key, register __d0 DES_key_schedule *schedule)
{
	DES_set_key_unchecked(key, schedule);
}

__asm __saveds void des_string_to_key_AmiSSL(register __a0 const char *str, register __a1 DES_cblock *key)
{
	DES_string_to_key(str, key);
}

__asm __saveds void des_string_to_2keys_AmiSSL(register __a0 const char *str, register __a1 DES_cblock *key1, register __a2 DES_cblock *key2)
{
	DES_string_to_2keys(str, key1, key2);
}

__asm __saveds void des_cfb64_encrypt_AmiSSL(register __a0 const unsigned char *in, register __a1 unsigned char *out, register __d0 long length, register __d1 DES_key_schedule *schedule, register __a2 DES_cblock *ivec, register __a3 int *num, register __d2 int enc)
{
	DES_cfb64_encrypt(in, out, length, schedule, ivec, num, enc);
}

__asm __saveds void des_ofb64_encrypt_AmiSSL(register __a0 const unsigned char *in, register __a1 unsigned char *out, register __d0 long length, register __d1 DES_key_schedule *schedule, register __a2 DES_cblock *ivec, register __a3 int *num)
{
	DES_ofb64_encrypt(in, out, length, schedule, ivec, num);
}

/* OBSOLETE */
 
__asm __saveds int des_read_pw_string_AmiSSL(register __a0 char *buf, register __d0 int length, register __a1 const char *prompt, register __d1 int verify)
{
//	return(DES_read_pw_string(buf, length, prompt, verify));
	return UI_UTIL_read_pw_string(buf, length, prompt, verify);
}

__asm __saveds int des_read_pw_AmiSSL(register __a0 char *buf, register __a1 char *buff, register __d0 int size, register __a2 const char *prompt, register __d1 int verify)
{
//	return(DES_read_pw(buf, buff, size, prompt, verify));
	return UI_UTIL_read_pw(buf, buff, size, prompt, verify);
}

__asm __saveds void des_random_seed_AmiSSL(register __a0 DES_cblock *key)
{
//	DES_random_seed(key);
	RAND_seed(key, sizeof(DES_cblock));
}
