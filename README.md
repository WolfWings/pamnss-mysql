This is a ground-up rebuild of both the libnss-mysql and pam-mysql projects,
targetting exclusively Linux and MySQL 5.x or higher to simplify the decade-
old code and clean it up using modern practices.

The configuration file, while similar, is wildly simplified to speed parsing
by requiring entire name/value pairs to be on a single line. It supports all
standard line-ending types (DOS, linux, Mac) and will attempt to return sane
error messages including line/character positions. Also all 'values' are now
limited to a single 4K page (including terminating byte, to 4095 characters)
so if you need a more complex query to assemble one of the views you'll need
to setup an appropriate 'CREATE VIEW' construct instead.

Also the example is geared to require the MySQL-tallied UIDs/GIDs to stay at
all times identical, I.E. there is only a single unique 'ID' field, returned
for queries of both the UID and GID. Arbitrary additional GID membership all
works still, but you cannot have a separate UID/GID by default.
