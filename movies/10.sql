SELECT DISTINCT(name)
  FROM people
       INNER JOIN directors
       ON directors.person_id = people.id

       INNER JOIN movies
       ON directors.movie_id = movies.id

       INNER JOIN ratings
       ON ratings.movie_id = movies.id
          AND ratings.rating >= 9;