# Tattle

List login records for a particular user or at a particular time

## How To Use

``` bash
tattle [-d date] [-f filename] [-t time] [-u login[,login]*]
```

## Options

| Option | Meaning                                                                    | Use                  |
| ------ | -------------------------------------------------------------------------- | -------------------- |
| d      | Restrict the report to logins active on the specified date                 | -d mm/dd/yy          |
| f      | Take data from a file                                                      | -f filename          |
| t      | Restrict the report to logins active at the specified time (24-hour clock) | -t HH:MM             |
| u      | Restrict the report to the specified logins                                | -u user1,user2,user3 |
