SELECT AVG(r.rating) AS average_rating
FROM movies m
JOIN ratings r ON m.id = r.movie_id
WHERE m.year = 2012;
