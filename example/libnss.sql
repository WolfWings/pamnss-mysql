CREATE DATABASE IF NOT EXISTS pamnss;
USE pamnss;

# The tables ...
CREATE TABLE grouplist (
  rowid INT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
  gid SMALLINT UNSIGNED NOT NULL DEFAULT '0',
  username VARCHAR(256) CHARACTER SET 'ascii' NOT NULL DEFAULT ''
);

# Force UID/GID to match for SQL-authed users
CREATE TABLE users (
  uidgid SMALLINT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
  name VARCHAR(256) CHARACTER SET 'ascii' NOT NULL DEFAULT '' UNIQUE,
  homedir VARCHAR(255) CHARACTER SET 'ascii' NOT NULL DEFAULT '',
  shell VARCHAR(255) CHARACTER SET 'ascii' NOT NULL DEFAULT '/bin/bash',
  password VARCHAR(110) CHARACTER SET 'ascii' NOT NULL DEFAULT 'x',
  flag BIGINT UNSIGNED NOT NULL DEFAULT '0'
) AUTO_INCREMENT = 5000;

# The data ...
INSERT INTO users (name,homedir,password) VALUES ('sqltest','/dev/null',ENCRYPT('sqltest','$6$AAAAAAAAAAAAAAAA'));
INSERT INTO grouplist (gid,username) VALUES(5000,'sqltest');

# The permissions ...
GRANT USAGE ON *.* TO `pam-root`@`%` IDENTIFIED BY 'password';
GRANT USAGE ON *.* TO `nss-root`@`%` IDENTIFIED BY 'password';
GRANT USAGE ON *.* TO `nss-user`@`%` IDENTIFIED BY 'password';

GRANT SELECT (`name`, `password`)
             ON `pamnss`.`users`
             TO 'pam-root'@'%';

GRANT SELECT (`name`, `uidgid`, `uidgid`, `homedir`, `shell`, `password`,
              `lastchanged`, `min`, `max`, `warn`, `graceperiod`, `expire`, `flag`)
             ON `pamnss`.`users`
             TO 'nss-root'@'%';
GRANT SELECT (`username`, `gid`)
             ON `pamnss`.`grouplist`
             TO 'nss-root'@'%';

GRANT SELECT (`name`, `uidgid`, `uidgid`, `homedir`, `shell`)
             ON `pamnss`.`users`
             TO 'nss-user'@'%';
GRANT SELECT (`username`, `gid`)
             ON `pamnss`.`grouplist`
             TO 'nss-user'@'%';
