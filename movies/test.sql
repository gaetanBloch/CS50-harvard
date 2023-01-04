SELECT count(name)), birth
  FROM people
       INNER JOIN stars
       ON stars.person_id = people.id
      AND people.birth = NULL

       INNER JOIN movies
       ON stars.movie_id = movies.id
          AND movies.year = 2004
 ORDER BY people.birth;