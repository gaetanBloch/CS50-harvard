-- Keep a log of any SQL queries you execute as you solve the mystery.

---- Clues from the spec ----
-- Male or Female or Other
-- Took place on 07/28/2021 -> No time
-- Took place on Humphrey Street
-- Criminal took a flight
---- Out of town -> Not in Fiftyville
---- Shortly after the crime -> I guess the same day
-- Has an accomplice
---> It's not stated whether he/she took the plane or not
---> It's highly probable they escape together by car straight to the airport

---- Checking the crime scene reports first
-- SELECT * FROM crime_scene_reports;

---- Checking the crime scene first the date of the heist (reports from 2021)
-- SELECT * FROM crime_scene_reports
-- WHERE month = 07
--       AND day = 28;

-- clue: The heist took place at 10:15am
-- clue: 3 witnesses interviewed the same day
-- clue: All wistnesses mentioned the bakery

---- Check the interviews of that day
-- SELECT name, transcript FROM interviews
-- WHERE month = 07
--       AND day = 28
--       AND transcript LIKE "%bakery%";

-- Ruth
--> Thief got into the car within 10 minutes >> 10:15 < t <= 10:25
--> Car was in bakery parking
--> Check security footages (1)
-- Eugene
--> Recognized the thief
--> Thief was spotted at ATM on Leggett Street withdrawing >> t < 10:15
--> Check ATM withdrawals (2)
-- Raymond
--> Thief & accomplice called someone for < 1 min
--> Both thief and accomplice are taking a flight the day after >> 07/29
--> A third accomplice on the phone bought the plane tickets
--> Check phone calls (3)
--> Check flights (4)

-- (1) -- Check the security footages of the bakery
-- SELECT license_plate FROM bakery_security_logs
-- WHERE month = 07
--       AND day = 28
--       AND hour = 10
--       AND minute > 15
--       AND minute <= 25
--       AND activity = "exit";

-- +---------------+
-- | license_plate |
-- +---------------+
-- | 5P2BI95       |
-- | 94KL13X       |
-- | 6P58WS2       |
-- | 4328GD8       |
-- | G412CB7       |
-- | L93JTIZ       |
-- | 322W7JE       |
-- | 0NTHK55       |
-- +---------------+

---- Check other times those license places where spotted
-- SELECT activity, year, month, day, hour, minute FROM bakery_security_logs
-- WHERE license_plate IN
--                     (SELECT license_plate FROM bakery_security_logs
--                     WHERE month = 07
--                           AND day = 28
--                           AND hour = 10
--                           AND minute > 15
--                           AND minute <= 25
--                           AND activity = "exit");

-- +----------+------+-------+-----+------+--------+
-- | activity | year | month | day | hour | minute |
-- +----------+------+-------+-----+------+--------+
-- | entrance | 2021 | 7     | 28  | 8    | 18     |
-- | entrance | 2021 | 7     | 28  | 8    | 23     |
-- | entrance | 2021 | 7     | 28  | 8    | 36     |
-- | entrance | 2021 | 7     | 28  | 8    | 42     |
-- | entrance | 2021 | 7     | 28  | 9    | 14     |
-- | entrance | 2021 | 7     | 28  | 9    | 15     |
-- | entrance | 2021 | 7     | 28  | 9    | 20     |
-- | entrance | 2021 | 7     | 28  | 9    | 28     |
-- | exit     | 2021 | 7     | 28  | 10   | 16     |
-- | exit     | 2021 | 7     | 28  | 10   | 18     |
-- | exit     | 2021 | 7     | 28  | 10   | 18     |
-- | exit     | 2021 | 7     | 28  | 10   | 19     |
-- | exit     | 2021 | 7     | 28  | 10   | 20     |
-- | exit     | 2021 | 7     | 28  | 10   | 21     |
-- | exit     | 2021 | 7     | 28  | 10   | 23     |
-- | exit     | 2021 | 7     | 28  | 10   | 23     |
-- +----------+------+-------+-----+------+--------+

---- Identify to whom belong the license plates

-- SELECT name, phone_number, passport_number, license_plate
-- FROM people
-- WHERE license_plate IN
--                     (SELECT license_plate FROM bakery_security_logs
--                     WHERE month = 07
--                           AND day = 28
--                           AND hour = 10
--                           AND minute > 15
--                           AND minute <= 25
--                           AND activity = "exit");

-- +---------+----------------+-----------------+---------------+
-- |  name   |  phone_number  | passport_number | license_plate |
-- +---------+----------------+-----------------+---------------+
-- | Vanessa | (725) 555-4692 | 2963008352      | 5P2BI95       |
-- | Barry   | (301) 555-4174 | 7526138472      | 6P58WS2       |
-- | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
-- | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
-- | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +---------+----------------+-----------------+---------------+


-- (2) -- Check ATM withdrahals
-- SELECT account_number, amount FROM atm_transactions
-- WHERE month = 07
--       AND day = 28
--       AND atm_location = "Leggett Street"
--       AND transaction_type = "withdraw";

-- +----------------+--------+
-- | account_number | amount |
-- +----------------+--------+
-- | 28500762       | 48     |
-- | 28296815       | 20     |
-- | 76054385       | 60     |
-- | 49610011       | 50     |
-- | 16153065       | 80     |
-- | 25506511       | 20     |
-- | 81061156       | 30     |
-- | 26013199       | 35     |
-- +----------------+--------+

---- Identify to whom belong the accounts
-- SELECT p.name, p.phone_number, p.passport_number, p.license_plate
-- FROM bank_accounts AS b
--      INNER JOIN people AS p
--      ON p.id = b.person_id
-- WHERE b.account_number IN
--                       (SELECT account_number FROM atm_transactions
--                       WHERE month = 07
--                             AND day = 28
--                             AND atm_location = "Leggett Street"
--                             AND transaction_type = "withdraw");

-- +---------+----------------+-----------------+---------------+
-- |  name   |  phone_number  | passport_number | license_plate |
-- +---------+----------------+-----------------+---------------+
-- | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | Brooke  | (122) 555-4581 | 4408372428      | QX4YZN3       |
-- | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
-- | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
-- | Benista | (338) 555-6650 | 9586786673      | 8X428L0       |
-- +---------+----------------+-----------------+---------------+

---- Check correlation betweeen ATM transaction and Parking Footage
-- SELECT name, phone_number, passport_number, license_plate
-- FROM people
-- WHERE license_plate IN
--                     (SELECT license_plate FROM bakery_security_logs
--                     WHERE month = 07
--                           AND day = 28
--                           AND hour = 10
--                           AND minute > 15
--                           AND minute <= 25
--                           AND activity = "exit")

-- AND id IN

--        (SELECT p.id
--        FROM bank_accounts AS b
--            INNER JOIN people AS p
--            ON p.id = b.person_id
--        WHERE b.account_number IN
--                              (SELECT account_number FROM atm_transactions
--                              WHERE month = 07
--                                    AND day = 28
--                                    AND atm_location = "Leggett Street"
--                                    AND transaction_type = "withdraw"));


------ IDENTIFIED 4 SUSPECTS (culprit)
-- They left the bakery by car between 10:15 and 10:25
-- They withdrew money the same day at the ATM in Leggett Street

-- +-------+----------------+-----------------+---------------+
-- | name  |  phone_number  | passport_number | license_plate |
-- +-------+----------------+-----------------+---------------+
-- | Iman  | (829) 555-5269 | 7049073643      | L93JTIZ       |
-- | Luca  | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | Diana | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +-------+----------------+-----------------+---------------+

------

-- (3) -- Check phone calls less than 1 minutes (60 seconds)
-- SELECT caller, receiver, duration FROM phone_calls
-- WHERE month = 07
--       AND day = 28
--       AND duration <= 60;

-- +----------------+----------------+----------+
-- |     caller     |    receiver    | duration |
-- +----------------+----------------+----------+
-- | (130) 555-0289 | (996) 555-8899 | 51       |
-- | (499) 555-9472 | (892) 555-8872 | 36       |
-- | (367) 555-5533 | (375) 555-8161 | 45       |
-- | (609) 555-5876 | (389) 555-5198 | 60       |
-- | (499) 555-9472 | (717) 555-1342 | 50       |
-- | (286) 555-6063 | (676) 555-6554 | 43       |
-- | (770) 555-1861 | (725) 555-3243 | 49       |
-- | (031) 555-6622 | (910) 555-3251 | 38       |
-- | (826) 555-1652 | (066) 555-9701 | 55       |
-- | (338) 555-6650 | (704) 555-2131 | 54       |
-- +----------------+----------------+----------+

-- Identify callers (either culprit or accomplice)
-- SELECT name, phone_number, passport_number, license_plate
-- FROM people
-- WHERE phone_number IN
--                    (SELECT caller FROM phone_calls
--                    WHERE month = 07
--                            AND day = 28
--                            AND duration <= 60);

-- +---------+----------------+-----------------+---------------+
-- |  name   |  phone_number  | passport_number | license_plate |
-- +---------+----------------+-----------------+---------------+
-- | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
-- | Benista | (338) 555-6650 | 9586786673      | 8X428L0       |
-- | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
-- | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | Kathryn | (609) 555-5876 | 6121106406      | 4ZY7I8T       |
-- | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | Carina  | (031) 555-6622 | 9628244268      | Q12B3Z3       |
-- +---------+----------------+-----------------+---------------+

-- Identify receivers (the one who bought the plane tickets)
-- SELECT name, phone_number, passport_number, license_plate
-- FROM people
-- WHERE phone_number IN
--                    (SELECT receiver FROM phone_calls
--                    WHERE month = 07
--                            AND day = 28
--                            AND duration <= 60);

-- +------------+----------------+-----------------+---------------+
-- |    name    |  phone_number  | passport_number | license_plate |
-- +------------+----------------+-----------------+---------------+
-- | James      | (676) 555-6554 | 2438825627      | Q13SVG6       |
-- | Larry      | (892) 555-8872 | 2312901747      | O268ZZ0       |
-- | Luca       | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | Anna       | (704) 555-2131 |                 |               |
-- | Jack       | (996) 555-8899 | 9029462229      | 52R0Y8U       |
-- | Melissa    | (717) 555-1342 | 7834357192      |               |
-- | Jacqueline | (910) 555-3251 |                 | 43V0R5D       |
-- | Philip     | (725) 555-3243 | 3391710505      | GW362R6       |
-- | Robin      | (375) 555-8161 |                 | 4V16VO0       |
-- | Doris      | (066) 555-9701 | 7214083635      | M51FA04       |
-- +------------+----------------+-----------------+---------------+

---- Check correlation 4 identified suspects and callers
-- SELECT id, name, phone_number, passport_number, license_plate
-- FROM people
-- WHERE phone_number IN
--                    (SELECT caller FROM phone_calls
--                    WHERE month = 07
--                            AND day = 28
--                            AND duration <= 60)
-- AND id IN

--         (SELECT id
--         FROM people
--         WHERE license_plate IN
--                             (SELECT license_plate FROM bakery_security_logs
--                             WHERE month = 07
--                                 AND day = 28
--                                 AND hour = 10
--                                 AND minute > 15
--                                 AND minute <= 25
--                                 AND activity = "exit")

--         AND id IN

--             (SELECT p.id
--             FROM bank_accounts AS b
--                 INNER JOIN people AS p
--                 ON p.id = b.person_id
--             WHERE b.account_number IN
--                                     (SELECT account_number FROM atm_transactions
--                                     WHERE month = 07
--                                         AND day = 28
--                                         AND atm_location = "Leggett Street"
--                                         AND transaction_type = "withdraw")));



------ IDENTIFIED 2 SUSPECTS
-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 514354 | Diana | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+-------+----------------+-----------------+---------------+
-------


-- Check who Diana called
-- SELECT id, name, phone_number, passport_number, license_plate
-- FROM people
-- WHERE phone_number IN
--                    (SELECT receiver FROM phone_calls
--                    WHERE month = 07
--                            AND day = 28
--                            AND duration <= 60
--                            AND caller = "(770) 555-1861");

-- +--------+--------+----------------+-----------------+---------------+
-- |   id   |  name  |  phone_number  | passport_number | license_plate |
-- +--------+--------+----------------+-----------------+---------------+
-- | 847116 | Philip | (725) 555-3243 | 3391710505      | GW362R6       |
-- +--------+--------+----------------+-----------------+---------------+

-- Check who Bruce called
-- SELECT id, name, phone_number, passport_number, license_plate
-- FROM people
-- WHERE phone_number IN
--                    (SELECT receiver FROM phone_calls
--                    WHERE month = 07
--                            AND day = 28
--                            AND duration <= 60
--                            AND caller = "(367) 555-5533");

-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 864400 | Robin | (375) 555-8161 |                 | 4V16VO0       |
-- +--------+-------+----------------+-----------------+---------------+

---->
----> Philip or Robin booked 2 tickets for the culprit and his accomplice
--====> WRONG: THEY BOOKED 1 ticket and one of them is the accomplice
---->

-- (4) -- Check the first flight from fiftyville the 29th of July
-- SELECT f.id,
--        origin_airport_id AS origin,
--        destination_airport_id AS destination,
--        abbreviation,
--        city
-- FROM flights AS f
--      INNER JOIN airports AS a
--      ON a.id = f.origin_airport_id
--         AND a.city LIKE "fiftyville"
--         AND f.month = 7
--         AND f.day = 29
-- ORDER BY hour, minute
-- LIMIT 1;


------ IDENTIFIED FLIGHT THEY TOOK TO ESCAPE

-- +----+--------+-------------+--------------+------------+
-- | id | origin | destination | abbreviation |    city    |
-- +----+--------+-------------+--------------+------------+
-- | 36 | 8      | 4           | CSF          | Fiftyville |
-- +----+--------+-------------+--------------+------------+

--------

-- Check their destination
-- SELECT full_name, city FROM airports
-- WHERE id =(SELECT f.destination_airport_id
--           FROM flights AS f
--               INNER JOIN airports AS a
--               ON a.id = f.origin_airport_id
--                   AND a.city LIKE "fiftyville"
--                   AND f.month = 7
--                   AND f.day = 29
--           ORDER BY hour
--           LIMIT 1);

------ IDENTIFIED DESTINATION -> NEW YORK CITY

-- +-------------------+---------------+
-- |     full_name     |     city      |
-- +-------------------+---------------+
-- | LaGuardia Airport | New York City |
-- +-------------------+---------------+

-------

-- Check passengers of the flight
-- SELECT passport_number, seat FROM passengers
-- WHERE flight_id = (SELECT f.id
--           FROM flights AS f
--               INNER JOIN airports AS a
--               ON a.id = f.origin_airport_id
--                   AND a.city LIKE "fiftyville"
--                   AND f.month = 7
--                   AND f.day = 29
--           ORDER BY hour
--           LIMIT 1);

-- +-----------------+------+
-- | passport_number | seat |
-- +-----------------+------+
-- | 7214083635      | 2A   |
-- | 1695452385      | 3B   |
-- | 5773159633      | 4A   |
-- | 1540955065      | 5C   |
-- | 8294398571      | 6C   |
-- | 1988161715      | 6D   |
-- | 9878712108      | 7A   |
-- | 8496433585      | 7B   |
-- +-----------------+------+

-- Identify passengers
-- SELECT name, phone_number, passport_number, license_plate
-- FROM people
-- WHERE passport_number IN
--                         (SELECT passport_number FROM passengers
--                         WHERE flight_id = (SELECT f.id
--                                 FROM flights AS f
--                                     INNER JOIN airports AS a
--                                     ON a.id = f.origin_airport_id
--                                         AND a.city LIKE "fiftyville"
--                                         AND f.month = 7
--                                         AND f.day = 29
--                                 ORDER BY hour
--                                 LIMIT 1));

-- +--------+----------------+-----------------+---------------+
-- |  name  |  phone_number  | passport_number | license_plate |
-- +--------+----------------+-----------------+---------------+
-- | Kenny  | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | Sofia  | (130) 555-0289 | 1695452385      | G412CB7       |
-- | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
-- | Luca   | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | Kelsey | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | Edward | (328) 555-1152 | 1540955065      | 130LD9Z       |
-- | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | Doris  | (066) 555-9701 | 7214083635      | M51FA04       |
-- +--------+----------------+-----------------+---------------+

-- Find culprit in List of passengers

-- SELECT name, phone_number, passport_number, license_plate
-- FROM people
-- WHERE passport_number IN
--                         (SELECT passport_number FROM passengers
--                         WHERE flight_id = (SELECT f.id
--                                 FROM flights AS f
--                                     INNER JOIN airports AS a
--                                     ON a.id = f.origin_airport_id
--                                         AND a.city LIKE "fiftyville"
--                                         AND f.month = 7
--                                         AND f.day = 29
--                                 ORDER BY hour
--                                 LIMIT 1))

-- AND id IN

-- (SELECT id
-- FROM people
-- WHERE phone_number IN
--                    (SELECT caller FROM phone_calls
--                    WHERE month = 07
--                            AND day = 28
--                            AND duration <= 60)
-- AND id IN

--         (SELECT id
--         FROM people
--         WHERE license_plate IN
--                             (SELECT license_plate FROM bakery_security_logs
--                             WHERE month = 07
--                                 AND day = 28
--                                 AND hour = 10
--                                 AND minute > 15
--                                 AND minute <= 25
--                                 AND activity = "exit")

--         AND id IN

--             (SELECT p.id
--             FROM bank_accounts AS b
--                 INNER JOIN people AS p
--                 ON p.id = b.person_id
--             WHERE b.account_number IN
--                                     (SELECT account_number FROM atm_transactions
--                                     WHERE month = 07
--                                         AND day = 28
--                                         AND atm_location = "Leggett Street"
--                                         AND transaction_type = "withdraw"))));

--------- CULPRIT IS BRUCE -----------

-- +-------+----------------+-----------------+---------------+
-- | name  |  phone_number  | passport_number | license_plate |
-- +-------+----------------+-----------------+---------------+
-- | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +-------+----------------+-----------------+---------------+

-----------------------------------------

-- To find the culprit we just need to check who Bruce called as we discovered
-- before it was ROBIN, with this query:

-- Check who Bruce called
-- SELECT id, name, phone_number, passport_number, license_plate
-- FROM people
-- WHERE phone_number IN
--                    (SELECT receiver FROM phone_calls
--                    WHERE month = 07
--                            AND day = 28
--                            AND duration <= 60
--                            AND caller = "(367) 555-5533");


--------- ACCOMPLICE IS ROBIN -----------

-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 864400 | Robin | (375) 555-8161 |                 | 4V16VO0       |
-- +--------+-------+----------------+-----------------+---------------+

---------