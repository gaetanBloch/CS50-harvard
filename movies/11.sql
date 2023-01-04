SELECT movies.title
  FROM movies
       INNER JOIN stars
       ON stars.movie_id = movies.id

       INNER JOIN people
       ON people.id = stars.person_id
          AND people.name LIKE "chadwick boseman"

       INNER JOIN ratings
       ON ratings.movie_id = movies.id
 ORDER BY ratings.rating DESC
 LIMIT 5;