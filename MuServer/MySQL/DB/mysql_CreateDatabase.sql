-- ----------------------------------------------------------------------------
-- MySQL Workbench Migration
-- Migrated Schemata: MuOnline97
-- Source Schemata: MuOnline97
-- Created: Tue Feb 13 21:35:28 2024
-- Workbench Version: 8.0.36
-- ----------------------------------------------------------------------------

SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------------------------------------------------------
-- Schema MuOnline97
-- ----------------------------------------------------------------------------
DROP SCHEMA IF EXISTS `MuOnline97` ;
CREATE SCHEMA IF NOT EXISTS `MuOnline97` ;

-- ----------------------------------------------------------------------------
-- Table MuOnline97.AccountCharacter
-- ----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS `MuOnline97`.`AccountCharacter` (
	`Id` VARCHAR(10) NOT NULL,
	`GameID1` VARCHAR(10) NULL,
	`GameID2` VARCHAR(10) NULL,
	`GameID3` VARCHAR(10) NULL,
	`GameID4` VARCHAR(10) NULL,
	`GameID5` VARCHAR(10) NULL,
	`GameIDC` VARCHAR(10) NULL,
	PRIMARY KEY (`Id`)
);

-- ----------------------------------------------------------------------------
-- Table MuOnline97.Character
-- ----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS `MuOnline97`.`Character` (
	`AccountID` VARCHAR(10) NOT NULL,
	`Name` VARCHAR(10) NOT NULL,
	`ResetCount` INT UNSIGNED NOT NULL DEFAULT 0,
	`GrandResetCount` INT UNSIGNED NOT NULL DEFAULT 0,
	`cLevel` INT UNSIGNED NOT NULL DEFAULT 1,
	`LevelUpPoint` INT UNSIGNED NULL DEFAULT 0,
	`Class` TINYINT UNSIGNED NULL,
	`Experience` INT UNSIGNED NULL DEFAULT 0,
	`Strength` INT UNSIGNED NULL,
	`Dexterity` INT UNSIGNED NULL,
	`Vitality` INT UNSIGNED NULL,
	`Energy` INT UNSIGNED NULL,
	`Inventory` VARBINARY(760) NULL,
	`MagicList` VARBINARY(180) NULL,
	`Money` INT UNSIGNED NULL DEFAULT 0,
	`Life` INT UNSIGNED NULL,
	`MaxLife` INT UNSIGNED NULL,
	`Mana` INT UNSIGNED NULL,
	`MaxMana` INT UNSIGNED NULL,
	`BP` INT UNSIGNED NULL,
	`MaxBP` INT UNSIGNED NULL,
	`MapNumber` TINYINT UNSIGNED NULL,
	`MapPosX` TINYINT UNSIGNED NULL,
	`MapPosY` TINYINT UNSIGNED NULL,
	`MapDir` TINYINT UNSIGNED NULL DEFAULT 0,
	`PkCount` INT UNSIGNED NULL DEFAULT 0,
	`PkLevel` INT NULL DEFAULT 3,
	`PkTime` INT UNSIGNED NULL DEFAULT 0,
	`CtlCode` TINYINT UNSIGNED NULL DEFAULT 0,
	`Bloc_Expire` DATETIME NULL,
	`DbVersion` TINYINT UNSIGNED NULL DEFAULT 0,
	`Quest` VARBINARY(50) NULL DEFAULT 0,
	`ChatLimitTime` INT UNSIGNED NULL DEFAULT 0,
	`FruitPoint` INT NULL DEFAULT 0,
	`EffectList` VARBINARY(208) NULL,
	`FruitAddPoint` INT NOT NULL DEFAULT 0,
	`FruitSubPoint` INT NOT NULL DEFAULT 0,
	PRIMARY KEY (`Name`)
);

-- ----------------------------------------------------------------------------
-- Table MuOnline97.DefaultClassType
-- ----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS `MuOnline97`.`DefaultClassType` (
	`Class` TINYINT UNSIGNED NOT NULL,
	`Level` INT UNSIGNED NULL DEFAULT 0,
	`LevelUpPoint` INT UNSIGNED NULL DEFAULT 0,
	`Strength` INT UNSIGNED NULL,
	`Dexterity` INT UNSIGNED NULL,
	`Vitality` INT UNSIGNED NULL,
	`Energy` INT UNSIGNED NULL,
	`Inventory` VARBINARY(760) NULL,
	`MagicList` VARBINARY(180) NULL,
	`Life` INT UNSIGNED NULL,
	`MaxLife` INT UNSIGNED NULL,
	`Mana` INT UNSIGNED NULL,
	`MaxMana` INT UNSIGNED NULL,
	`MapNumber` TINYINT UNSIGNED NULL,
	`MapPosX` TINYINT UNSIGNED NULL,
	`MapPosY` TINYINT UNSIGNED NULL,
	`Quest` VARBINARY(50) NULL,
	`DbVersion` TINYINT UNSIGNED NULL,
	`EffectList` VARBINARY(208) NULL,
	PRIMARY KEY (`Class`)
);

-- ----------------------------------------------------------------------------
-- Table MuOnline97.ExtWarehouse
-- ----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS `MuOnline97`.`ExtWarehouse` (
	`AccountID` VARCHAR(10) NOT NULL,
	`Number` INT UNSIGNED NULL,
	`Items` VARBINARY(1920) NULL,
	`Money` INT UNSIGNED NULL
);

-- ----------------------------------------------------------------------------
-- Table MuOnline97.GameServerInfo
-- ----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS `MuOnline97`.`GameServerInfo` (
	`Number` INT UNSIGNED NOT NULL DEFAULT 0,
	`ItemCount` INT UNSIGNED NOT NULL,
	PRIMARY KEY (`Number`)
);

-- ----------------------------------------------------------------------------
-- Table MuOnline97.GoldenCoin
-- ----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS `MuOnline97`.`GoldenCoin` (
	`AccountID` VARCHAR(50) NOT NULL,
	`RenaCount` INT UNSIGNED NOT NULL,
	`StoneCount` INT UNSIGNED NOT NULL,
	`LuckyNumber` VARCHAR(12) NULL
);

-- ----------------------------------------------------------------------------
-- Table MuOnline97.Guild
-- ----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS `MuOnline97`.`Guild` (
	`Number` INT UNSIGNED NOT NULL AUTO_INCREMENT,
	`G_Name` VARCHAR(8) NOT NULL,
	`G_Mark` VARBINARY(32) NULL,
	`G_Score` INT UNSIGNED NULL DEFAULT 0,
	`G_Master` VARCHAR(10) NULL,
	`G_Notice` VARCHAR(60) NULL,
	`MemberCount` INT UNSIGNED NULL DEFAULT 0,
	PRIMARY KEY (`Number`)
);

-- ----------------------------------------------------------------------------
-- Table MuOnline97.GuildMember
-- ----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS `MuOnline97`.`GuildMember` (
	`Name` VARCHAR(10) NOT NULL,
	`G_Name` VARCHAR(8) NOT NULL,
	`G_Status` TINYINT UNSIGNED NOT NULL DEFAULT 0,
	PRIMARY KEY (`Name`)
);

-- ----------------------------------------------------------------------------
-- Table MuOnline97.MEMB_INFO
-- ----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS `MuOnline97`.`MEMB_INFO` (
	`memb___id` VARCHAR(10) NOT NULL,
	`memb__pwd` VARCHAR(10) NOT NULL,
	`memb_name` VARCHAR(10) NOT NULL,
	`sno__numb` CHAR(18) NOT NULL,
	`bloc_code` CHAR(1) NOT NULL,
	`AccountLevel` INT NOT NULL DEFAULT 0,
	`AccountExpireDate` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
	`Bloc_Expire` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
	`mail_addr` VARCHAR(50) NULL,
	`ctl1_code` CHAR(1) NOT NULL DEFAULT 0,
	PRIMARY KEY (`memb___id`)
);

-- ----------------------------------------------------------------------------
-- Table MuOnline97.MEMB_STAT
-- ----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS `MuOnline97`.`MEMB_STAT` (
	`memb___id` VARCHAR(10) NOT NULL,
	`ConnectStat` TINYINT UNSIGNED NULL,
	`ServerName` VARCHAR(50) NULL,
	`IP` VARCHAR(15) NULL,
	`ConnectTM` DATETIME NULL,
	`DisConnectTM` DATETIME NULL,
	`OnlineHours` INT UNSIGNED NULL DEFAULT 0,
	PRIMARY KEY (`memb___id`)
);

-- ----------------------------------------------------------------------------
-- Table MuOnline97.OptionData
-- ----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS `MuOnline97`.`OptionData` (
	`Name` VARCHAR(10) NOT NULL,
	`SkillKey` BINARY(10) NULL,
	`GameOption` TINYINT UNSIGNED NULL,
	`Qkey` TINYINT UNSIGNED NULL,
	`Wkey` TINYINT UNSIGNED NULL,
	`Ekey` TINYINT UNSIGNED NULL,
	`ChatWindow` TINYINT UNSIGNED NULL,
	PRIMARY KEY (`Name`)
);

-- ----------------------------------------------------------------------------
-- Table MuOnline97.RankingBloodCastle
-- ----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS `MuOnline97`.`RankingBloodCastle` (
	`Name` VARCHAR(10) NOT NULL,
	`Score` INT UNSIGNED NULL,
	PRIMARY KEY (`Name`)
);

-- ----------------------------------------------------------------------------
-- Table MuOnline97.RankingDevilSquare
-- ----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS `MuOnline97`.`RankingDevilSquare` (
	`Name` VARCHAR(10) NOT NULL,
	`Score` INT UNSIGNED NULL,
	PRIMARY KEY (`Name`)
);

-- ----------------------------------------------------------------------------
-- Table MuOnline97.ResetInfo
-- ----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS `MuOnline97`.`ResetInfo` (
	`Name` VARCHAR(10) NOT NULL,
	`ResetDay` INT UNSIGNED NOT NULL DEFAULT 0,
	`ResetDayDate` DATETIME NULL,
	`ResetWek` INT UNSIGNED NOT NULL DEFAULT 0,
	`ResetWekDate` DATETIME NULL,
	`ResetMon` INT UNSIGNED NOT NULL DEFAULT 0,
	`ResetMonDate` DATETIME NULL,
	`GrandResetDay` INT UNSIGNED NOT NULL DEFAULT 0,
	`GrandResetDayDate` DATETIME NULL,
	`GrandResetWek` INT UNSIGNED NOT NULL DEFAULT 0,
	`GrandResetWekDate` DATETIME NULL,
	`GrandResetMon` INT UNSIGNED NOT NULL DEFAULT 0,
	`GrandResetMonDate` DATETIME NULL,
	PRIMARY KEY (`Name`)
);

-- ----------------------------------------------------------------------------
-- Table MuOnline97.warehouse
-- ----------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS `MuOnline97`.`warehouse` (
	`AccountID` VARCHAR(10) NOT NULL,
	`Items` VARBINARY(1200) NULL,
	`Money` INT UNSIGNED NULL DEFAULT 0,
	`EndUseDate` DATETIME NULL,
	`DbVersion` TINYINT UNSIGNED NULL DEFAULT 0,
	`pw` SMALLINT NULL DEFAULT 0,
	PRIMARY KEY (`AccountID`)
);

-- ----------------------------------------------------------------------------
-- Poblate table MuOnline97.DefaultClassType with default values
-- ----------------------------------------------------------------------------

INSERT `MuOnline97`.`DefaultClassType` (Class, Level, LevelUpPoint, Strength, Dexterity, Vitality, Energy, Inventory, MagicList, Life, MaxLife, Mana, MaxMana, MapNumber, MapPosX, MapPosY, Quest, DbVersion, EffectList) VALUES (0, 1, 0, 18, 18, 15, 30,  0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 60, 60, 60, 60, 0, 125, 125, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 3, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF);
INSERT `MuOnline97`.`DefaultClassType` (Class, Level, LevelUpPoint, Strength, Dexterity, Vitality, Energy, Inventory, MagicList, Life, MaxLife, Mana, MaxMana, MapNumber, MapPosX, MapPosY, Quest, DbVersion, EffectList) VALUES (16, 1, 0, 28, 20, 25, 10, 0x20001200000000000001FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 110, 110, 20, 20, 0, 125, 125, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 3, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF);
INSERT `MuOnline97`.`DefaultClassType` (Class, Level, LevelUpPoint, Strength, Dexterity, Vitality, Energy, Inventory, MagicList, Life, MaxLife, Mana, MaxMana, MapNumber, MapPosX, MapPosY, Quest, DbVersion, EffectList) VALUES (32, 1, 0, 22, 25, 20, 15, 0x8F00FF0000000000000480001400000000000004FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 80, 80, 30, 30, 3, 172, 97, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 3, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF);
INSERT `MuOnline97`.`DefaultClassType` (Class, Level, LevelUpPoint, Strength, Dexterity, Vitality, Energy, Inventory, MagicList, Life, MaxLife, Mana, MaxMana, MapNumber, MapPosX, MapPosY, Quest, DbVersion, EffectList) VALUES (48, 1, 0, 26, 26, 26, 26, 0x01001600000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 110, 110, 60, 60, 0, 125, 125, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 3, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF);

-- ----------------------------------------------------------------------------
-- Routine MuOnline97.WZ_CONNECT_MEMB
-- ----------------------------------------------------------------------------
DELIMITER $$

CREATE PROCEDURE `MuOnline97`.`WZ_CONNECT_MEMB`(
  IN memb_id varchar(10),
  IN ServerName varchar(50),
  IN IP varchar(20)
)
BEGIN
  SET @find_id = 'NOT';
  SET @ConnectStat = 1;

  SELECT S.memb___id INTO @find_id
  FROM `MuOnline97`.`MEMB_STAT` S INNER JOIN `MuOnline97`.`MEMB_INFO` I ON S.memb___id = I.memb___id
  WHERE I.memb___id = `memb_id`;

  IF (@find_id = 'NOT') THEN
	INSERT INTO `MuOnline97`.`MEMB_STAT` (memb___id, ConnectStat, ServerName, IP, ConnectTM)
	VALUES (`memb_id`, @ConnectStat, `ServerName`, `IP`, NOW());
  ELSE
	UPDATE `MuOnline97`.`MEMB_STAT`
	SET ConnectStat = @ConnectStat, ServerName = `ServerName`, IP = `IP`, ConnectTM = NOW()
	WHERE memb___id = `memb_id`;
  END IF;
END $$

DELIMITER ;

-- ----------------------------------------------------------------------------
-- Routine MuOnline97.WZ_CreateCharacter
-- ----------------------------------------------------------------------------
DELIMITER $$

CREATE PROCEDURE `MuOnline97`.`WZ_CreateCharacter`(
	IN AccountID VARCHAR(10),
	IN CharName VARCHAR(10),
	IN CharClass TINYINT
)
BEGIN
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		ROLLBACK;
		SELECT 0 AS Result;
	END;

	-- Results cases:
	-- -- Return 1 if success
	-- -- Return 0 will show: 'Invalid character name or name supplied already exists.'
	-- -- Return 2 will show: 'No more characters can be created.'
	SET @Result = 1;

	IF EXISTS (SELECT 1 FROM `MuOnline97`.`Character` WHERE Name = `CharName`) THEN
		SET @Result = 0;
	ELSE
		START TRANSACTION;
		IF NOT EXISTS (SELECT 1 FROM `MuOnline97`.`AccountCharacter` WHERE Id = `AccountID`) THEN
			INSERT INTO `MuOnline97`.`AccountCharacter` (Id, GameID1) VALUES (`AccountID`, `CharName`);
		ELSE
			SELECT GameID1, GameID2, GameID3, GameID4, GameID5
			INTO @g1, @g2, @g3, @g4, @g5
			FROM `MuOnline97`.`AccountCharacter`
			WHERE Id = `AccountID`;

			IF @g1 IS NULL OR LENGTH(@g1) = 0 THEN
				UPDATE `MuOnline97`.`AccountCharacter` SET GameID1 = `CharName` WHERE Id = `AccountID`;
			ELSEIF @g2 IS NULL OR LENGTH(@g2) = 0 THEN
				UPDATE `MuOnline97`.`AccountCharacter` SET GameID2 = `CharName` WHERE Id = `AccountID`;
			ELSEIF @g3 IS NULL OR LENGTH(@g3) = 0 THEN
				UPDATE `MuOnline97`.`AccountCharacter` SET GameID3 = `CharName` WHERE Id = `AccountID`;
			ELSEIF @g4 IS NULL OR LENGTH(@g4) = 0 THEN
				UPDATE `MuOnline97`.`AccountCharacter` SET GameID4 = `CharName` WHERE Id = `AccountID`;
			ELSEIF @g5 IS NULL OR LENGTH(@g5) = 0 THEN
				UPDATE `MuOnline97`.`AccountCharacter` SET GameID5 = `CharName` WHERE Id = `AccountID`;
			ELSE
				SET @Result = 2;
			END IF;
		END IF;

		IF @Result <> 1 THEN
			ROLLBACK;
		ELSE
			INSERT INTO `MuOnline97`.`Character` (AccountID, Name, cLevel, LevelUpPoint, Class, Strength, Dexterity, Vitality, Energy, Inventory, MagicList, Life, MaxLife, Mana, MaxMana, BP, MaxBP, MapNumber, MapPosX, MapPosY, Quest, DbVersion, EffectList)
			SELECT `AccountID`, `CharName`, Level, LevelUpPoint, `CharClass`, Strength, Dexterity, Vitality, Energy, Inventory, MagicList, Life, MaxLife, Mana, MaxMana, 0, 0, MapNumber, MapPosX, MapPosY, Quest, DbVersion, EffectList
			FROM `MuOnline97`.`DefaultClassType` WHERE Class = `CharClass`;
			COMMIT;
		END IF;
	END IF;

	SELECT @Result AS Result;
END $$

DELIMITER ;

-- ----------------------------------------------------------------------------
-- Routine MuOnline97.WZ_DeleteCharacter
-- ----------------------------------------------------------------------------
DELIMITER $$

CREATE PROCEDURE `MuOnline97`.`WZ_DeleteCharacter`(
	IN Account VARCHAR(10),
	IN CharName VARCHAR(10)
)
BEGIN
	DECLARE CONTINUE HANDLER FOR SQLEXCEPTION SELECT 1 AS Result;

	-- Results cases:
	-- -- Return 1 if success
	-- -- Return 0 will show: 'You cannot delete a character with guild.'
	-- -- Return 2 will show: 'You have entered an invalid personal ID number.'
	-- -- Return 3 will show: 'This is an item-blocked character.'
	SET @Result = 0;

	IF NOT EXISTS (SELECT 1 FROM `MuOnline97`.`Character` WHERE Name = `CharName`) THEN
		SET @Result = 0;
	ELSE
		SET @Result = 1;

		DELETE FROM `MuOnline97`.`Character` WHERE AccountID = `Account` AND Name = `CharName`;
		DELETE FROM `MuOnline97`.`ResetInfo` WHERE Name = `CharName`;
		DELETE FROM `MuOnline97`.`OptionData` WHERE Name = `CharName`;
		DELETE FROM `MuOnline97`.`RankingBloodCastle` WHERE Name = `CharName`;
		DELETE FROM `MuOnline97`.`RankingDevilSquare` WHERE Name = `CharName`;
	END IF;

	SELECT @Result AS Result;
END $$

DELIMITER ;

-- ----------------------------------------------------------------------------
-- Routine MuOnline97.WZ_DesblocAccount
-- ----------------------------------------------------------------------------
DELIMITER $$

CREATE PROCEDURE `MuOnline97`.`WZ_DesblocAccount`(
	IN Account VARCHAR(10)
)
BEGIN
	UPDATE `MuOnline97`.`MEMB_INFO`
	SET bloc_code = 0
	WHERE memb___id = `Account` AND bloc_code = 1 AND bloc_expire < NOW();
END $$

DELIMITER ;

-- ----------------------------------------------------------------------------
-- Routine MuOnline97.WZ_DesblocCharacters
-- ----------------------------------------------------------------------------
DELIMITER $$

CREATE PROCEDURE `MuOnline97`.`WZ_DesblocCharacters`(
	IN Account VARCHAR(10)
)
BEGIN
	UPDATE `MuOnline97`.`Character`
	SET CtlCode = 0
	WHERE AccountID = `Account` AND CtlCode = 1 AND bloc_expire < NOW();
END $$

DELIMITER ;

-- ----------------------------------------------------------------------------
-- Routine MuOnline97.WZ_DISCONNECT_MEMB
-- ----------------------------------------------------------------------------
DELIMITER $$

CREATE PROCEDURE `MuOnline97`.`WZ_DISCONNECT_MEMB`(
	IN memb_id VARCHAR(10)
)
BEGIN
	UPDATE `MuOnline97`.`MEMB_STAT`
	SET ConnectStat = 0, DisConnectTM = NOW(), OnlineHours = OnlineHours + TIMESTAMPDIFF(HOUR, ConnectTM, NOW())
	WHERE memb___id = `memb_id`;
END $$

DELIMITER ;

-- ----------------------------------------------------------------------------
-- Routine MuOnline97.WZ_GetAccountLevel
-- ----------------------------------------------------------------------------
DELIMITER $$

CREATE PROCEDURE `MuOnline97`.`WZ_GetAccountLevel` (
	IN Account VARCHAR(10)
)
BEGIN
	SELECT AccountLevel, AccountExpireDate INTO @CurrentAccountLevel, @CurrentAccountExpireDate
	FROM `MuOnline97`.`MEMB_INFO`
	WHERE memb___id = `Account`;

	IF (@CurrentAccountLevel <> 0 AND NOW() >= @CurrentAccountExpireDate) THEN
		SET @CurrentAccountLevel = 0;

		UPDATE `MuOnline97`.`MEMB_INFO`
		SET AccountLevel = 0
		WHERE memb___id = `Account`;
	END IF;

	SELECT @CurrentAccountLevel AS AccountLevel, @CurrentAccountExpireDate AS AccountExpireDate;
END $$

DELIMITER ;

-- ----------------------------------------------------------------------------
-- Routine MuOnline97.WZ_GetGrandResetInfo
-- ----------------------------------------------------------------------------
DELIMITER $$

CREATE PROCEDURE `MuOnline97`.`WZ_GetGrandResetInfo` (
	IN Account VARCHAR(10),
	IN CharName VARCHAR(10)
)
BEGIN
	IF NOT EXISTS (SELECT * FROM `MuOnline97`.`ResetInfo` WHERE Name = `CharName`) THEN
		INSERT INTO `MuOnline97`.`ResetInfo` (Name) VALUES (`CharName`);
	END IF;

	SELECT GrandResetCount INTO @GrandReset
	FROM `MuOnline97`.`Character`
	WHERE AccountID = `Account` AND Name = `CharName`;

	SELECT GrandResetDay, GrandResetDayDate, GrandResetWek, GrandResetWekDate, GrandResetMon, GrandResetMonDate
	INTO @GrandResetDay, @GrandResetDayDate, @GrandResetWek, @GrandResetWekDate, @GrandResetMon, @GrandResetMonDate
	FROM `MuOnline97`.`ResetInfo`
	WHERE Name = `CharName`;

	IF DATEDIFF(NOW(), @GrandResetDayDate) > 0 THEN
		SET @GrandResetDay = 0;
		UPDATE `MuOnline97`.`ResetInfo` SET GrandResetDay = 0 WHERE Name = `CharName`;
	END IF;

	IF DATEDIFF(NOW(), @GrandResetWekDate) > 0 THEN
		SET @GrandResetWek = 0;
		UPDATE `MuOnline97`.`ResetInfo` SET GrandResetWek = 0 WHERE Name = `CharName`;
	END IF;

	IF DATEDIFF(NOW(), @GrandResetMonDate) > 0 THEN
		SET @GrandResetMon = 0;
		UPDATE `MuOnline97`.`ResetInfo` SET GrandResetMon = 0 WHERE Name = `CharName`;
	END IF;

	SELECT @GrandReset AS GrandReset, @GrandResetDay AS GrandResetDay, @GrandResetWek AS GrandResetWek, @GrandResetMon AS GrandResetMon;
END $$

DELIMITER ;

-- ----------------------------------------------------------------------------
-- Routine MuOnline97.WZ_GetItemSerial
-- ----------------------------------------------------------------------------
DELIMITER $$

CREATE PROCEDURE `MuOnline97`.`WZ_GetItemSerial`()
BEGIN
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		ROLLBACK;
		SELECT -1 as Result;
	END;

	START TRANSACTION;

	SELECT ItemCount INTO @ItemSerial
	FROM `MuOnline97`.`GameServerInfo`
	WHERE Number = 0;

	IF (@ItemSerial >= 2147483647) THEN -- MAX INTEGER VALUE
		SET @ItemSerial = 1;
	ELSE
		SET @ItemSerial = @ItemSerial + 1;
	END IF;

	UPDATE `MuOnline97`.`GameServerInfo`
	SET ItemCount = @ItemSerial
	WHERE Number = 0;

	IF (ROW_COUNT() = 0) THEN
		ROLLBACK;
		SELECT -1 as Result;
	ELSE
		COMMIT;
		SELECT @ItemSerial as Result;
	END IF;
END $$

DELIMITER ;

-- ----------------------------------------------------------------------------
-- Routine MuOnline97.WZ_GetResetInfo
-- ----------------------------------------------------------------------------
DELIMITER $$

CREATE PROCEDURE `MuOnline97`.`WZ_GetResetInfo`(
	IN Account VARCHAR(10),
	IN CharName VARCHAR(10)
)
BEGIN
	IF NOT EXISTS (SELECT * FROM `MuOnline97`.`ResetInfo` WHERE Name = `CharName`) THEN
		INSERT INTO `MuOnline97`.`ResetInfo` (Name) VALUES (`CharName`);
	END IF;

	SELECT ResetCount INTO @ResetCount
	FROM `MuOnline97`.`Character`
	WHERE AccountID = `Account` AND Name = `CharName`;

	SELECT ResetDay, ResetDayDate, ResetWek, ResetWekDate, ResetMon, ResetMonDate
	INTO @ResetDay, @ResetDayDate, @ResetWek, @ResetWekDate, @ResetMon, @ResetMonDate
	FROM `MuOnline97`.`ResetInfo`
	WHERE Name = `CharName`;

	IF DATEDIFF(NOW(), @ResetDayDate) > 0 THEN
		SET @ResetDay = 0;
		UPDATE `MuOnline97`.`ResetInfo` SET ResetDay = 0 WHERE Name = `CharName`;
	END IF;

	IF DATEDIFF(NOW(), @ResetWekDate) > 0 THEN
		SET @ResetWek = 0;
		UPDATE `MuOnline97`.`ResetInfo` SET ResetWek = 0 WHERE Name = `CharName`;
	END IF;

	IF DATEDIFF(NOW(), @ResetMonDate) > 0 THEN
		SET @ResetMon = 0;
		UPDATE `MuOnline97`.`ResetInfo` SET ResetMon = 0 WHERE Name = `CharName`;
	END IF;

	SELECT @ResetCount AS Reset, @ResetDay AS ResetDay, @ResetWek AS ResetWek, @ResetMon AS ResetMon;
END $$

DELIMITER ;

-- ----------------------------------------------------------------------------
-- Routine MuOnline97.WZ_GuildCreate
-- ----------------------------------------------------------------------------
DELIMITER $$

CREATE PROCEDURE `MuOnline97`.`WZ_GuildCreate`(
	IN GuildName VARCHAR(8),
	IN MasterName VARCHAR(10)
)
BEGIN
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		ROLLBACK;
		SELECT 6 AS Result;
	END;

	-- Results cases:
	-- -- Return 1 if success
	-- -- Return 0 will show: 'The guild name already exists.'
	-- -- Return 2 will show: 'The guild name must be at least 2 characters long.'
	-- -- Return 3 will show: 'You are already in a guild.'
	-- -- Return 4 will show: 'No space allowed in guild names.'
	-- -- Return 5 will show: 'No symbols allowed in guild names.'
	-- -- Return 6 will show: 'Create guild failed.'
	SET @ErrorCode = 1;

	START TRANSACTION;

	INSERT INTO `MuOnline97`.`Guild` (G_Name, G_Master)
	VALUES (`GuildName`, `MasterName`);

	IF (ROW_COUNT() = 0) THEN
		SET @ErrorCode = 6;
	ELSE
		INSERT INTO `MuOnline97`.`GuildMember` (Name, G_Name)
		VALUES (`MasterName`, `GuildName`);

		IF (ROW_COUNT() = 0) THEN
			SET @ErrorCode = 6;
		END IF;
	END IF;

	IF (@ErrorCode <> 1) THEN
		ROLLBACK;
	ELSE
		COMMIT;
	END IF;

	SELECT @ErrorCode as Result;
END $$

DELIMITER ;

-- ----------------------------------------------------------------------------
-- Routine MuOnline97.WZ_SetAccountLevel
-- ----------------------------------------------------------------------------
DELIMITER $$

CREATE PROCEDURE `MuOnline97`.`WZ_SetAccountLevel`(
	IN Account VARCHAR(10),
	IN AccountLevel INT,
	IN AccountExpireTime INT
)
BEGIN
	SELECT AccountLevel, AccountExpireDate INTO @CurrentAccountLevel, @CurrentAccountExpireDate
	FROM `MuOnline97`.`MEMB_INFO`
	WHERE memb___id = `Account`;

	IF (@CurrentAccountLevel = `AccountLevel`) THEN
		SET @CurrentAccountExpireDate = DATE_ADD(@CurrentAccountExpireDate, INTERVAL `AccountExpireTime` SECOND);
	ELSE
		SET @CurrentAccountLevel = `AccountLevel`;
		SET @CurrentAccountExpireDate = DATE_ADD(NOW(), INTERVAL `AccountExpireTime` SECOND);
	END IF;

	UPDATE `MuOnline97`.`MEMB_INFO`
	SET AccountLevel = @CurrentAccountLevel, AccountExpireDate = @CurrentAccountExpireDate
	WHERE memb___id = `Account`;
END $$

DELIMITER ;

-- ----------------------------------------------------------------------------
-- Routine MuOnline97.WZ_SetGrandResetInfo
-- ----------------------------------------------------------------------------
DELIMITER $$

CREATE PROCEDURE `MuOnline97`.`WZ_SetGrandResetInfo`(
	IN Account VARCHAR(10),
	IN Name VARCHAR(10),
	IN Reset INT,
	IN GrandReset INT,
	IN GrandResetDay INT,
	IN GrandResetWek INT,
	IN GrandResetMon INT
)
BEGIN
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		ROLLBACK;
	END;

	START TRANSACTION;

	UPDATE `MuOnline97`.`Character`
	SET ResetCount = `Reset`, GrandResetCount = `GrandReset`
	WHERE AccountID = `Account` AND Name = `Name`;

	UPDATE `MuOnline97`.`ResetInfo`
	SET GrandResetDay = `GrandResetDay`, GrandResetDayDate = NOW(),
		GrandResetWek = `GrandResetWek`, GrandResetWekDate = NOW(),
		GrandResetMon = `GrandResetMon`, GrandResetMonDate = NOW()
	WHERE Name = `Name`;

	COMMIT;
END $$

DELIMITER ;

-- ----------------------------------------------------------------------------
-- Routine MuOnline97.WZ_SetGuildDelete
-- ----------------------------------------------------------------------------
DELIMITER $$

CREATE PROCEDURE `MuOnline97`.`WZ_SetGuildDelete`(
	IN GuildName VARCHAR(10)
)
BEGIN
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		ROLLBACK;
		SELECT 3 AS Result;
	END;

	-- Results cases:
	-- -- Return 1 if success on member, will show 'You have just left the guild.'
	-- -- Return 4 if success on master, will show: 'The guild has been dissolved'
	-- -- Return 0 will show: 'Your personal ID number is incorrect.'
	-- -- Return 2 will show: 'Only the guild master can disband a guild.'
	-- -- Return 3 will show: 'There was an error deleting the guild'
	SET @Result = 1;

	START TRANSACTION;

	DELETE FROM `MuOnline97`.`GuildMember`
	WHERE G_Name = `GuildName`;

	IF (ROW_COUNT() = 0) THEN
		SET @Result = 3;
	ELSE
		DELETE FROM `MuOnline97`.`Guild`
		WHERE G_Name = `GuildName`;

		IF (ROW_COUNT() = 0) THEN
			SET @Result = 3;
		END IF;
	END IF;

	IF (@Result <> 1) THEN
		ROLLBACK;
	ELSE
		COMMIT;
	END IF;

	SELECT @Result AS Result;
END $$

DELIMITER ;

-- ----------------------------------------------------------------------------
-- Routine MuOnline97.WZ_SetResetInfo
-- ----------------------------------------------------------------------------
DELIMITER $$

CREATE PROCEDURE `MuOnline97`.`WZ_SetResetInfo`(
	IN Account VARCHAR(10),
	IN Name VARCHAR(10),
	IN Reset INT,
	IN ResetDay INT,
	IN ResetWek INT,
	IN ResetMon INT
)
BEGIN
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
	BEGIN
		ROLLBACK;
	END;

	START TRANSACTION;

	UPDATE `MuOnline97`.`Character`
	SET ResetCount = `Reset`
	WHERE AccountID = `Account` AND Name = `Name`;

	UPDATE `ResetInfo`
	SET ResetDay = `ResetDay`, ResetDayDate = NOW(),
		ResetWek = `ResetWek`, ResetWekDate = NOW(),
		ResetMon = `ResetMon`, ResetMonDate = NOW()
	WHERE Name = `Name`;

	COMMIT;
END $$

DELIMITER ;

-- ----------------------------------------------------------------------------
-- Routine MuOnline97.UFN_MD5_CHECKVALUE -> NEEDS TRANSLATION TO MYSQL
-- ----------------------------------------------------------------------------
-- SET ANSI_NULLS ON
-- GO
-- SET QUOTED_IDENTIFIER ON
-- GO

-- CREATE FUNCTION [dbo].[UFN_MD5_CHECKVALUE]
-- (
--   @btInStr VARCHAR(10),
--   @btInStrIndex VARCHAR(10),
--   @btInVal BINARY(16)
-- )
-- RETURNS TINYINT 
-- AS  
-- BEGIN 
--   DECLARE @iOutResult TINYINT

--   EXEC master..XP_MD5_CheckValue @btInStr, @btInVal, @btInStrIndex, @iOutResult OUT

--   RETURN @iOutResult
-- END
-- GO
-- ----------------------------------------------------------------------------
-- Routine MuOnline97.UFN_MD5_ENCODEVALUE -> NEEDS TRANSLATION TO MYSQL
-- ----------------------------------------------------------------------------
-- SET ANSI_NULLS ON
-- GO
-- SET QUOTED_IDENTIFIER ON
-- GO

-- CREATE FUNCTION [dbo].[UFN_MD5_ENCODEVALUE]
-- (
--   @btInStr VARCHAR(10),
--   @btInStrIndex VARCHAR(10)
-- )
-- RETURNS BINARY(16)
-- AS  
-- BEGIN 
--   DECLARE @btOutVal BINARY(16)

--   EXEC master..XP_MD5_EncodeKeyVal @btInStr, @btInStrIndex, @btOutVal OUT

--   RETURN @btOutVal
-- END
-- GO