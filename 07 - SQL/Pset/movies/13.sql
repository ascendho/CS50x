SELECT DISTINCT p2.name
FROM people p1
JOIN stars s1 ON p1.id = s1.person_id
JOIN stars s2 ON s1.movie_id = s2.movie_id
JOIN people p2 ON s2.person_id = p2.id
WHERE p1.name = 'Kevin Bacon'
    AND p1.birth = 1958
    AND p2.id != p1.id;
