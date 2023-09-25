SELECT title, year FROM movies
WHERE title LIKE 'Harry Potter%' AND NOT title = 'Harry Potter und ein Stein' AND NOT title = 'Harry Potter and the Cursed Child'
ORDER BY year ASC;