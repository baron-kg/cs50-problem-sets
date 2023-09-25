SELECT DISTINCT name FROM people
JOIN stars ON people.id = stars.person_id
WHERE stars.movie_id IN
(SELECT movies.id FROM movies
JOIN stars ON stars.movie_id = movies.id
JOIN people ON stars.person_id = people.id
WHERE people.name = 'Kevin Bacon' AND people.birth = 1958) AND NOT people.name = 'Kevin Bacon';