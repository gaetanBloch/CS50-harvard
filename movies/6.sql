SELECT AVG(rating) AS average_rating
  FROM ratings
       INNER JOIN movies
       ON ratings.movie_id = movies.id
          AND movies.year = 2012;