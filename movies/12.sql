-- SELECT movies.title
--   FROM movies
--        INNER JOIN stars
--        ON stars.movie_id = movies.id

--        INNER JOIN people
--        ON people.id = stars.person_id
--           AND people.name LIKE "johnny depp"

-- INTERSECT

-- SELECT movies.title
--   FROM movies
--        INNER JOIN stars
--        ON stars.movie_id = movies.id

--        INNER JOIN people
--        ON people.id = stars.person_id
--           AND people.name LIKE "helena bonham carter";


SELECT movies.title
  FROM movies
       INNER JOIN stars
       ON stars.movie_id = movies.id

       INNER JOIN people
       ON people.id = stars.person_id
          AND people.name LIKE "johnny depp"

WHERE movies.title IN

      (SELECT movies.title
         FROM movies
              INNER JOIN stars
              ON stars.movie_id = movies.id

              INNER JOIN people
              ON people.id = stars.person_id
                  AND people.name LIKE "helena bonham carter");