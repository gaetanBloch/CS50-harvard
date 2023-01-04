SELECT DISTINCT(name)
  FROM people
       INNER JOIN stars
       ON stars.person_id = people.id

       INNER JOIN movies
       ON stars.movie_id = movies.id
          AND movies.year = 2004
 ORDER BY people.birth;