# Web Server (API Apps/application) in pure C

## documentation

this API using JSON-RPC as data format.
please learn it before continue in this documentation.
for limit and offset it use same concept as in sql limit and offset

note: please look carefully about data type int or string mean alot.


* forever start --spinSleepTime 500 -c build/api.run api.sock

example:

- send

```javascript
	{
		"jsonrpc": "2.0",
		"method": "school.get", 
		"params": {
			"id": 1
		}, 
		"id": 12131
	}
```

- retrieve

```javascript
	{
		"jsonrpc": "2.0",
		"result": {
			id: 1,
			code: 1001,
			name: 'SDN Gegerkalong KPAD',
			level: 'elementary' 
		}, 
		"id": 12131
	}
```

## 

### Util API
just an utility, for ping (checking connection maybe)
	
#### ping
- sent:
	- method:
		<strong>ping</strong>
	- params:
		(no params)
- receive:
	- string "ok"

example sent:

```javascript
	{
		"jsonrpc": "2.0",
		"method": "ping", 
		"id": 421321
	}
```

example retrieve:

```javascript
	{
		"jsonrpc": "2.0",
		"result": "ok", 
		"id": 421321
	}
```


## 
#### author:
- ryan hs <mr.ryansilalahi@gmail.com>

## 
#### License:
MIT License
