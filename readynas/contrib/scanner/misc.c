//=========================================================================
// FILENAME	: misc.c
// DESCRIPTION	: Miscelleneous funcs
//=========================================================================
// Copyright (c) 2008- NETGEAR, Inc. All Rights Reserved.
//=========================================================================

/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <string.h>
#include <endian.h>
#include <openssl/sha.h>

#include "misc.h"

inline __u16
le16_to_cpu(__u16 le16)
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
  return le16;
#else
  __u16 be16 = ((le16<<8) & 0xff00) | ((le16>>8) & 0x00ff);
  return be16;
#endif
}

inline __u32
le32_to_cpu(__u32 le32)
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
  return le32;
#else
  __u32 be32 =
    ((le32<<24) & 0xff000000) |
    ((le32<< 8) & 0x00ff0000) |
    ((le32>> 8) & 0x0000ff00) |
    ((le32>>24) & 0x000000ff);
  return be32;
#endif
}

inline __u64
le64_to_cpu(__u64 le64)
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
  return le64;
#else
  __u64 be64;
  __u8 *le64p = (__u8*) &le64;
  __u8 *be64p = (__u8*) &be64;
  be64p[0] = le64p[7];
  be64p[1] = le64p[6];
  be64p[2] = le64p[5];
  be64p[3] = le64p[4];
  be64p[4] = le64p[3];
  be64p[5] = le64p[2];
  be64p[6] = le64p[1];
  be64p[7] = le64p[0];
  return be64;
#endif
}

inline __u8
fget_byte(FILE *fp)
{
  __u8 d;
  (void) fread(&d, sizeof(d), 1, fp);
  return d;
}

inline __u16
fget_le16(FILE *fp)
{
  __u16 d;
  (void) fread(&d, sizeof(d), 1, fp);
  d = le16_to_cpu(d);
  return d;
}

inline __u32
fget_le32(FILE *fp)
{
  __u32 d;
  (void) fread(&d, sizeof(d), 1, fp);
  d = le32_to_cpu(d);
  return d;
}

inline __u32
cpu_to_be32(__u32 cpu32)
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
  __u32 be32 =
    ((cpu32<<24) & 0xff000000) |
    ((cpu32<< 8) & 0x00ff0000) |
    ((cpu32>> 8) & 0x0000ff00) |
    ((cpu32>>24) & 0x000000ff);
  return be32;
#else
  return cpu32;
#endif
}


static unsigned char hexdigit[] = "0123456789abcdef";

char *
sha1_hex(char *key)
{
  unsigned char md[SHA_DIGEST_LENGTH];
  static char md_str[SHA_DIGEST_LENGTH*2+1];
  int i;

  SHA1((unsigned char*) key, strlen((char*) key), md);
  for (i=0; i<SHA_DIGEST_LENGTH; i++) {
    md_str[(i<<1)+0] = hexdigit[(md[i]>>4)&15];
    md_str[(i<<1)+1] = hexdigit[(md[i]>>0)&15];
  }
  md_str[SHA_DIGEST_LENGTH<<1] = '\0';

  return md_str;
}
