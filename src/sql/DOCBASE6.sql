--
-- DATABASE............> DOCBASE6
--
--
--
--

USE DOCBASE6;

CREATE TABLE `DOCENTITY` (
  `ET_IDENT` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `ET_DATA` date NOT NULL,
  `ET_ATIVO` tinyint(4) NOT NULL DEFAULT 1,
  `ET_CATEG` tinyint(4) NOT NULL DEFAULT 0,
  `ET_NOME` varchar(60) NOT NULL COMMENT 'NAME OR CORPORATE NAME',
  `ET_ALIAS` varchar(30) DEFAULT NULL,
  `ET_ENDER` varchar(60) NOT NULL COMMENT 'ADDRESS',
  `ET_ADDRNUMBER` int(11) DEFAULT 0,
  `ET_BAIR` varchar(60) NOT NULL COMMENT 'DISTRICT',
  `ET_CITY` varchar(60) NOT NULL,
  `ET_UF` char(2) DEFAULT NULL COMMENT 'STATE ABBREV',
  `ET_POSTALCODE` char(10) DEFAULT NULL,
  `ET_CATPHONE1` tinyint(4) DEFAULT 0,
  `ET_TELNUM1` varchar(30) DEFAULT NULL COMMENT 'PHONE NUMNER',
  `ET_CATPHONE2` tinyint(4) DEFAULT 0,
  `ET_TELNUM2` varchar(30) DEFAULT NULL,
  `ET_CATPHONE3` tinyint(4) DEFAULT 0,
  `ET_TELNUM3` varchar(30) DEFAULT NULL,
  `ET_CATPHONE4` tinyint(4) DEFAULT 0,
  `ET_TELNUM4` varchar(30) DEFAULT NULL,
  `ET_CONTATO1` varchar(40) DEFAULT NULL COMMENT 'CONSTACT NAME',
  `ET_EMAIL1` varchar(255) DEFAULT NULL,
  `ET_CONTATO2` varchar(40) DEFAULT NULL,
  `ET_EMAIL2` varchar(255) DEFAULT NULL,
  `ET_BR_CPF` varchar(19) DEFAULT NULL,
  `ET_BR_RG` varchar(21) DEFAULT NULL,
  `ET_BR_CNPJ` varchar(19) DEFAULT NULL,
  `ET_BR_IE` varchar(20) DEFAULT NULL,
  `ET_BR_IM` varchar(20) DEFAULT NULL,
  `ET_OTHERDOCTYPE` varchar(30) DEFAULT NULL,
  `ET_URL` varchar(255) DEFAULT NULL,
  `ET_OBCOD` smallint(6) DEFAULT NULL COMMENT 'PROJECT CODE',
  `ET_OBANO` smallint(5) unsigned NOT NULL DEFAULT 0 COMMENT 'PROJECT YEAR',
  `ET_OBCLI` bigint(20) unsigned NOT NULL DEFAULT 0 COMMENT 'COMMENTS',
  `ET_OBIDPROJ` varchar(20) DEFAULT NULL,
  `ET_OBCRONO` smallint(6) NOT NULL DEFAULT 0,
  `ET_OUTESPEC` varchar(50) DEFAULT NULL,
  `ET_OUTNFLD1` varchar(15) DEFAULT NULL,
  `ET_OUTDFLD1` varchar(255) DEFAULT NULL,
  `ET_OUTNFLD2` varchar(15) DEFAULT NULL,
  `ET_OUTDFLD2` varchar(255) DEFAULT NULL,
  `ET_OUTOBS` varchar(255) DEFAULT NULL,
  `ET_OBSGER` text DEFAULT NULL,
  PRIMARY KEY (`ET_IDENT`),
  KEY `ET_DATA` (`ET_DATA`),
  KEY `ET_ATIVO` (`ET_ATIVO`),
  KEY `ET_CATEG` (`ET_CATEG`),
  KEY `ET_NOME` (`ET_NOME`),
  KEY `ET_ALIAS` (`ET_ALIAS`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='DOCBASE::ENTITIES';
--
-- ***********************************************************************************************************
--
CREATE TABLE `DOCDEPARTMENTS` (
  `DP_IDENT` int(11) NOT NULL AUTO_INCREMENT,
  `DP_NAME` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`DP_IDENT`),
  KEY `DP_NAME` (`DP_NAME`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='DOCBASE::DEPARTMENTS';
--
-- ***********************************************************************************************************
--
CREATE TABLE `DOCSECURITY` (
  `ID` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `OP_NAME` varchar(15) DEFAULT NULL,
  `OP_PASSWD` blob DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `OP_PASSWD` (`OP_PASSWD`(3072))
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='DOCBASE::OPERATORS';

-- NOTE: If you want to change the keyword in the function
--       DES_ENCRYPT(), then you must do the same in the system source code.
--       Default: Username: admin Password: admin
INSERT INTO DOCSECURITY (OP_NAME, OP_PASSWD) VALUES ('admin',DES_ENCRYPT('admin','mE025@EZ+3.K'));
--
-- ***********************************************************************************************************
--
CREATE TABLE `DOCSYSACTIVITY` (
  `ACT_ID` int(11) NOT NULL AUTO_INCREMENT,
  `ACT_TIMESTAMP` timestamp NOT NULL DEFAULT current_timestamp(),
  `ACT_HOSTNAME` varchar(255) DEFAULT NULL,
  `ACT_USER` varchar(30) DEFAULT NULL,
  `ACT_MODULE` smallint(6) DEFAULT -1,
  `ACT_OPERATION` tinyint(4) DEFAULT -1,
  `ACT_DOCID` bigint(20) unsigned DEFAULT NULL,
  PRIMARY KEY (`ACT_ID`),
  KEY `ACT_TIMESTAMP` (`ACT_TIMESTAMP`),
  KEY `ACT_USER` (`ACT_USER`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='DOCBASE::OPERATIONS LOG';
--
-- ***********************************************************************************************************
--
CREATE TABLE `DOCSTORAGE` (
  `ST_IDENT` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `ST_RECDATE` date NOT NULL DEFAULT curdate(),
  `ST_OPERATOR` varchar(15) NOT NULL,
  `ET_IDENT` bigint(20) unsigned NOT NULL,
  `ST_ETRELA` bigint(20) unsigned NOT NULL,
  `ST_DOCDATE` date NOT NULL DEFAULT '0000-00-00',
  `ST_NUMBER` int(10) unsigned NOT NULL DEFAULT 0,
  `ST_VALUE` decimal(12,2) NOT NULL DEFAULT 0.00,
  `ST_DOCTYPE` tinyint(4) DEFAULT NULL,
  `ST_DOCIDENT` varchar(20) DEFAULT NULL,
  `ST_DEPART` varchar(15) DEFAULT NULL,
  `ST_FILE` varchar(255) DEFAULT NULL,
  `ST_FILETYPE` tinyint(1) DEFAULT NULL COMMENT 'Whether it is a tax document or not',
  `ST_REMARK` text NOT NULL,
  `ST_TOPSEC` tinyint(4) NOT NULL DEFAULT 0 COMMENT 'Whether protected by password or not',
  `ST_FIELD01DATA` varchar(255) DEFAULT NULL,
  `ST_FIELD02DATA` varchar(255) DEFAULT NULL,
  `ST_FIELD03DATA` varchar(255) DEFAULT NULL,
  `ST_FIELD04DATA` varchar(255) DEFAULT NULL,
  `ST_FIELD05DATA` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`ST_IDENT`),
  KEY `ST_TYPE` (`ST_TYPE`),
  KEY `ST_NUMBER` (`ST_NUMBER`),
  KEY `ET_IDENT` (`ET_IDENT`),
  KEY `ST_ETRELA` (`ST_ETRELA`),
  CONSTRAINT `DOCSTORAGE` FOREIGN KEY (`ET_IDENT`) REFERENCES `DOCENTITY` (`ET_IDENT`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='DOCBASE::DOCUMENT STORAGE';
--
-- ***********************************************************************************************************
--
CREATE TABLE `DOCIMAGES` (
  `IM_IDENT` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `ST_IDENT` bigint(20) unsigned NOT NULL,
  `IM_IMAGE` longblob DEFAULT NULL,
  PRIMARY KEY (`IM_IDENT`),
  KEY `DL_IDENT` (`ST_IDENT`),
  CONSTRAINT `DOCIMAGES_ibfk_1` FOREIGN KEY (`ST_IDENT`) REFERENCES `DOCSTORAGE` (`ST_IDENT`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='DOCBASE::DIGITALIZED FILES';
