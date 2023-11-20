# Test Tools for Key/Value Stores

NeoMutt uses a key/value store, e.g. lmdb, as a cache for email headers.

This repo contains some test tools to work with these stores.

## lmdb-dump.c

Dump the contents of an LMDB store.

**build**:
```sh
gcc lmdb-dump.c -o lmdb-dump -llmdb
```

**run**:
```sh
./lmdb-dump store-file
```

**output**:
```
Opening store-file
key: /home/flatcap/mail/1679930513.M731552P185191.flatcap.org,S=5071,W=5169
	data length: 999
key: /home/flatcap/mail/1679932108.M761797P185751.flatcap.org,S=5055,W=5153
	data length: 1008
...
54569 records

