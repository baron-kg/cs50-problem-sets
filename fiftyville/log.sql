-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Look up crime scene report on the crime in question.
SELECT description FROM crime_scene_reports
WHERE month = 7 AND day = 28 AND street = 'Humphrey Street';

-- The theft of the duck took place at 10:15AM at the Humphrey Street bakery. There were 3 witnesses all of which mention the bakery.

-- I look up the interviews of the 3 witnesses at the crime scene.
SELECT transcript FROM interviews
WHERE day = 28 AND month = 7;

-- One witness states that the thief left the bakery within 10 minutes of the theft and got into a car in the bakery parking lot.
-- The second witness states that he reconized the thief as a person that was withdrawing money earlier in the day at an ATM at Legget Street.
-- The third witness says he overheard the thief make a call that lasted less then a minute, in which he was planning to take the earliest flight out of the town. The thief asked the person on the phone to purchase a ticket for him.

-- Check the ATM transactions that happened at Legget Street at the day of the theft.
SELECT * FROM atm_transactions
WHERE month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw';

-- Get a list of suspects based on the account numbers that withdrew money on the day of the theft on the Legget Street ATM.
SELECT DISTINCT name FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
WHERE atm_transactions.account_number IN
(SELECT account_number FROM atm_transactions WHERE month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw');

-- The above query produced the following list of suspects: Bruce, Diana, Brooke, Kenny, Iman, Luca, Taylor and Benista.

-- Check the security camera footage.
SELECT activity, license_plate, hour, minute, year FROM bakery_security_logs
WHERE month = 7 AND day = 28 AND hour = 10 AND minute > 10 AND minute < 25;

-- Create a list of suspects based on licence plates that left the bakery parking lot between 10:15 AM and 10:25 AM.
SELECT DISTINCT name FROM people
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
WHERE bakery_security_logs.license_plate IN
(SELECT license_plate FROM bakery_security_logs
WHERE month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25 AND activity = 'exit');

-- The above query produced the following suspects: Iman, Bruce, Diana, Kelsey, Luca, Vanessa, Barry, Sofia.

-- Find the people that made a call that lasted less than a minute on the day of the crime.
SELECT DISTINCT name AS caller_name, receiver FROM people
JOIN phone_calls ON phone_calls.caller = people.phone_number
WHERE caller IN
(SELECT phone_number FROM people WHERE phone_calls.year = 2021 AND phone_calls.month = 7 AND phone_calls.day = 28 AND phone_calls.duration < 60);

-- The query producted the following suspects: Sofia, Kelsey, Bruce, Taylor, Diana, Carina, Kenny, Benista.

-- Find the people that recieved a call on the day of the crime.
SELECT name, caller, receiver FROM people
JOIN phone_calls ON phone_calls.receiver = people.phone_number
WHERE receiver IN
(SELECT phone_number FROM people WHERE phone_calls.year = 2021 AND phone_calls.month = 7 AND phone_calls.day = 28 AND phone_calls.duration < 60);

-- The query producet the following list of accomplices: Jack, Larry, Robin, Melissa, James, Philip, Jacqueline, Doris and Anna.

-- Display the people that left the bakery on the day of the crime within 10 minutes, withdrew money on the same day from Leggett Street and made a call that lasted less then a minute.
SELECT DISTINCT name FROM people
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
JOIN phone_calls ON phone_calls.caller = people.phone_number
WHERE bakery_security_logs.license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25 AND activity = 'exit')
AND atm_transactions.account_number IN
(SELECT account_number FROM atm_transactions WHERE month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw')
AND phone_calls.caller IN
(SELECT phone_number FROM people WHERE phone_calls.year = 2021 AND phone_calls.month = 7 AND phone_calls.day = 28 AND phone_calls.duration < 60);

-- The query narrowed down the suspects to Diana and Bruce.

-- Find out who Diana and Bruce called on the day of the crime.
SELECT * FROM phone_calls
WHERE month = 7 AND day = 28;

-- Find the aiport ID of Fiftyfille.
SELECT id FROM airports WHERE city = 'Fiftyville';

-- The ID is 8.

-- Check when is the earliest flight out of town on the 29th.
SELECT origin_airport_id, destination_airport_id, id, month, day, hour, minute FROM flights
WHERE origin_airport_id = 8 AND month = 7 AND day = 29
ORDER BY hour;

-- The earliest flight out of town is at 08:20 and the flight ID is 36

-- Find the names of passangers on the earliest flight out of town.
SELECT name FROM people
JOIN passengers ON passengers.passport_number = people.passport_number
JOIN flights ON flights.id = passengers.flight_id
WHERE passengers.flight_id IN (SELECT id FROM flights WHERE flights.id = 36);

-- The query returned a list of passangers where one name, Bruce, is also one of the two suspects which makes him the primary one.
-- Bruces phone number is (367) 555-5533

-- Check who talked to Bruce on the day of the crime
SELECT caller, receiver FROM phone_calls
WHERE month = 7 AND day = 28 AND duration < 60 AND caller = '(367) 555-5533';

-- Bruce called the number (375) 555-8161

-- Who does the above number belong to?
SELECT name FROM people
WHERE phone_number = '(375) 555-8161';

-- The accomplice was Robin!

-- What town did Bruce flee to?
SELECT DISTINCT city FROM airports
JOIN flights ON flights.origin_airport_id = airports.id
WHERE airports.id = (SELECT destination_airport_id FROM flights WHERE id = 36);

-- Bruce escaped to New York!