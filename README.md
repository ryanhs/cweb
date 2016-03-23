# PPDB API

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

### Content API
content api, category and post list.. why don't warming up a little before go to PPDB API :-)
	
#### get category list
- sent:
	- method:
		<strong>category.select</strong>
	- params:
		(no params)
- receive:
	- array (of category object):
		- int $id
		- string $name
		- string $permalink

example sent:

```javascript
	{
		"jsonrpc": "2.0",
		"method": "category.select", 
		"id": 321312
	}
```

example retrieve:

```javascript
	{
		"jsonrpc": "2.0",
		"result": [
			{
				"id": 1,
				"name": "uncategorized",
				"permalink": "uncategorized"
			},
			{
				"id": 2,
				"name": "News",
				"permalink": "news"
			}
		],
		"id": 321312
	}
```

	
#### get post list
- sent:
	- method:
		<strong>post.select</strong>
	- params:
		- int $id (id category)
		  
		  or
		  
		  string $permalink (permalink category)
		
		- (optional) int $limit limit retrieve, 0 mean all, default: 0
		- (optional) int $offset pagination ?, default: 0
- receive:
	- int $count counted all data (max data can be retrieve)
	- array $data (of post object):
		- int $id
		- string $subject
		- string $permalink
		- string $post_time
		- object $category
			- int $id
			- string $name
			- string $permalink
		
example sent 1:

```javascript
	{
		"jsonrpc": "2.0",
		"method": "post.select", 
		"params": {
			"permalink": "uncategorized"
		},
		"id": 543543
	}
```

example sent 2:

```javascript
	{
		"jsonrpc": "2.0",
		"method": "post.select", 
		"params": {
			"id": 1
		},
		"id": 63524
	}
```

example sent 3:

```javascript
	{
		"jsonrpc": "2.0",
		"method": "post.select", 
		"params": {
			"id": 1,
			"limit": 2,
			"offset": 1
		},
		"id": 645742
	}
```

example retrieve:

```javascript
	{
		"jsonrpc": "2.0",
		"result": {
			"count": 3,
			"data": [
				{
					"id": 2, 
					"subject": "first post", 
					"permalink": "first", 
					"post_time": "2015-03-25 17:09:31",
					"category": {
						"id": 1,
						"name": "uncategorized",
						"permalink": "uncategorized"
					}
				},
				{
					"id": 3, 
					"subject": "second post", 
					"permalink": "second", 
					"post_time": "2015-03-25 17:09:31",
					"category": {
						"id": 1,
						"name": "uncategorized",
						"permalink": "uncategorized"
					}
				}
			]
		},
		"id": 645742
	}
```

	
#### get post detail
- sent:
	- method:
		<strong>post.get</strong>
	- params:
		- int $id (id post)
		  
		  or
		  
		  string $permalink (permalink post)
		
		- (optional) string $type "minimal"
- receive:
	- int $id
	- string $subject
	- string $permalink
	- string $post_time
	- string $content if $type = "minimal", then this variable will not displayed
	- object $category
		- int $id
		- string $name
		- string $permalink
			

example sent:

```javascript
	{
		"jsonrpc": "2.0",
		"method": "post.get", 
		"params": {
			"permalink": "first"
		},
		"id": 21
	}
```

example retrieve:

```javascript
	{
		"jsonrpc": "2.0",
		"result": {
			"id": 2,
			"subject": "first post",
			"permalink": "first",
			"post_time": "2015-03-25 17:09:31",
			"category": {
				"id": 1,
				"name": "uncategorized",
				"permalink": "uncategorized"
			},
			"content": "first content!"
		},
		"id": 21
	}
```


## 

### PPDB API
	
#### get school list
- sent:
	- method:
		<strong>school.select</strong>
	- params:
		- string $level "elementary" or "junior" or "senior" or "vocational"
- receive:
	- array (of school object):
		- int $id
		- int $code
		- string $name
		- string $level, just ignore it :p

example sent:

```javascript
	{
		"jsonrpc": "2.0",
		"method": "school.select", 
		"params": {
			"level": "junior"
		},
		"id": 321
	}
```

example retrieve:

```javascript
	{
		"jsonrpc": "2.0",
		"result": [
			{
				"id": 325,
				"code": 2001,
				"name": "SMP Negeri 1",
				"level": "junior"
			},
			.
			.
			.
			{
				"id": 379,
				"code": 2055,
				"name": "MTsN Negeri 2",
				"level": "junior"
			}
		],
		"id": 321
	}
```
	
#### get school detail
- sent:
	- method:
		<strong>school.get</strong>
	- params:
		- int $id get from school list to get id
		- string $type "minimal" or "normal" or "full", how it will display data
- receive:
	- int $id
	- int $code
	- string $level
	- string $name
	- string $address (normal, full)
	- string $address\_district (normal, full)
	- int $address\_rw (normal, full)
	- int $address\_rt (normal, full)
	- int $is_border (normal, full) 1 = true, 0 = false
	- int $foreigner\_percentage (normal, full)
	- int $option\_i how many this school has option (full)
	- array (of option object) (full)
		- int $id
		- int $code
		- object $school just ignore it :-p
		- string $name
		- string $type "academic" or "mou" or "achievement" or "poor"
		
example sent:

```javascript
	{
		"jsonrpc": "2.0",
		"method": "school.select", 
		"params": {
			"id": 338,
			"type": "full"
		},
		"id": 32143
	}
```

example retrieve:

```javascript
	{
		"jsonrpc": "2.0",
		"result": {
			"id": 338,
			"code": 2014,
			"level": "junior",
			"name": "SMP Negeri 14",
			"address": "",
			"address_district": "BANDUNG WETAN",
			"address_subdistrict": "CIHAPIT",
			"address_rw": 6,
			"address_rt": 5,
			"is_border": 0,
			"foreigner_percentage": 10,
			"option_i": 4,
			"option": [
				{
					"id": 377,
					"school": {
						"id": 338,
						"code": 2014,
						"name": "SMP Negeri 14",
						"level": "junior"
					},
					"name": "AKADEMIK",
					"type": "academic"
				},
				.
				.
				.
				{
					"id": 380,
					"school": {
						"id": 338,
						"code": 2014,
						"name": "SMP Negeri 14",
						"level": "junior"
					},
					"name": "PRESTASI",
					"type": "achievement"
				}
			]
		},
		"id": 32143
	}
```

	


## 
#### author:
- ryan hs <mr.ryansilalahi@gmail.com>
