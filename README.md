This is a ground-up rebuild of both the libnss-mysql and pam-mysql projects,
targetting exclusively Linux and MySQL 5.x or higher to simplify the decade-
old code and clean it up using modern practices.

The configuration file, while similar, is wildly simplified to speed parsing
by requiring entire name/value pairs to be on a single line. It supports all
standard line-ending types (DOS, linux, Mac) and will attempt to return sane
error messages including line/character positions.
